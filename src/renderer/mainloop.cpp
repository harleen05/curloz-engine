#include "renderer/mainloop.hpp"
#include "clz_types.hpp"
#include "renderer/render_types.hpp"
#include "vulkan/vulkan_core.h"
#include "renderer/image.hpp"

namespace clz
{
namespace render
{
    clz::Result waitForGPU(const types::Device device, VkFence fence)
    {
        if (vkWaitForFences(device.device, 1, &fence, VK_TRUE, UINT64_MAX) != VK_SUCCESS)
            return clz::Failure("Failed to wait for fence");

        if (vkResetFences(device.device, 1, &fence) != VK_SUCCESS)
            return clz::Failure("Failed to reset fence");

        return clz::Success();
    }

    clz::Result acquireNextImage(const types::Device device, const types::Swapchain swapchain, VkSemaphore semaphore, uint32_t *pImageIndex)
    {
        VkResult acquireResult = vkAcquireNextImageKHR(device.device, swapchain.swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, pImageIndex);

        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
            return clz::Failure("renderer/mainloop: Swapchain is out of date");

        if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
            return clz::Failure("renderer/mainloop: Failed to acquire next image");

        return clz::Success();
    }

    clz::Result startCommandBuffer(const types::Device device, types::CommandBuffer commandBuffer)
    {
        if (vkResetCommandBuffer(commandBuffer.commandBuffer, 0) != VK_SUCCESS)
        {
            return clz::Failure("renderer/mainloop: Failed to reset command buffer");
        }
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffer.commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            return clz::Failure("renderer/mainloop: Failed to begin command buffer");
        }
        return clz::Success();
    }

    clz::Result recordCommandBuffer(const types::CommandBuffer commandBuffer, const types::Swapchain swapchain, uint32_t imageIndex, types::Pipeline pipeline)
    {

        transition_image_layout(
            swapchain.swapchainImages[imageIndex],
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            0,
            VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR,
            VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT_KHR,
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
            commandBuffer.commandBuffer
        );

        VkRenderingAttachmentInfoKHR colorAttachment{};
        colorAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR;
        colorAttachment.imageView = swapchain.swapchainImageViews[imageIndex];
        colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.clearValue = {{0.0f, 0.0f, 0.0f, 1.0f}};

        VkRenderingInfoKHR renderingInfo{};
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
        renderingInfo.renderArea = {{0, 0}, swapchain.extent};
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;

        vkCmdBeginRendering(commandBuffer.commandBuffer, &renderingInfo);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapchain.extent.width;
        viewport.height = (float)swapchain.extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer.commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{{0, 0}, swapchain.extent};
        vkCmdSetScissor(commandBuffer.commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline.pipeline);
        vkCmdDraw(commandBuffer.commandBuffer, 3, 1, 0, 0);

        vkCmdEndRendering(commandBuffer.commandBuffer);

        transition_image_layout(
            swapchain.swapchainImages[imageIndex],
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT_KHR,
            0,
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
            VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT_KHR,
            commandBuffer.commandBuffer
        );

        if (vkEndCommandBuffer(commandBuffer.commandBuffer) != VK_SUCCESS)
        {
            return clz::Failure("renderer/mainloop: vkEndCommandBuffer failed");
        }

        return clz::Success();
    }

    clz::Result submitCommandBuffer(types::Queue queue, types::CommandBuffer commandBuffer, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, VkFence inFlightFence)
    {
        VkSemaphoreSubmitInfoKHR waitSemaphore
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
            .semaphore = imageAvailableSemaphore,
            .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR
        };

        VkSemaphoreSubmitInfoKHR signalSemaphore
        {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO_KHR,
            .semaphore = renderFinishedSemaphore,
            .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT_KHR
        };

        VkCommandBufferSubmitInfoKHR cmdSubmitInfo{};
        cmdSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
        cmdSubmitInfo.commandBuffer = commandBuffer.commandBuffer;


        VkSubmitInfo2KHR submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2_KHR;
        submitInfo.waitSemaphoreInfoCount = 1;
        submitInfo.pWaitSemaphoreInfos = &waitSemaphore;
        submitInfo.commandBufferInfoCount = 1;
        submitInfo.pCommandBufferInfos = &cmdSubmitInfo;
        submitInfo.signalSemaphoreInfoCount = 1;
        submitInfo.pSignalSemaphoreInfos = &signalSemaphore;

        if (vkQueueSubmit2(queue.graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
            return clz::Failure("renderer/mainloop: vkQueueSubmit failed");

        return clz::Success();
    }
    clz::Result present(const types::Swapchain swapchain, const types::Queue queue, VkSemaphore semaphore, uint32_t imageIndex)
    {
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &semaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain.swapchain;
        presentInfo.pImageIndices = &imageIndex;

        if (vkQueuePresentKHR(queue.graphicsQueue, &presentInfo) != VK_SUCCESS)
            return clz::Failure("renderer/mainloop: vkQueuePresent failed");

        return clz::Success();
    }
}
}
