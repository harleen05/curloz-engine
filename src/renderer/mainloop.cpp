/**
 * @file mainloop.cpp
 * @brief Per-frame Vulkan render loop step implementations.
 *
 * Implements the six render loop steps declared in mainloop.hpp.
 * All functions operate on explicitly passed handles — no global state here.
 * Module-level state (r_currentFrame, r_imageIndex, etc.) lives in renderer.cpp.
 */

#include "renderer/mainloop.hpp"
#include "clz_types.hpp"
#include "renderer/render_types.hpp"
#include "vulkan/vulkan_core.h"
#include "renderer/image.hpp"

namespace clz
{
namespace render
{
	/**
	 * @brief Waits on the in-flight fence then resets it.
	 *
	 * Blocks indefinitely (UINT64_MAX timeout) until the GPU signals the fence,
	 * guaranteeing command buffer and sync primitives for this frame are free.
	 * Resets the fence so it can be re-signaled by the next vkQueueSubmit2.
	 *
	 * @param device Logical device.
	 * @param pFence Pointer to the fence to wait on and reset.
	 * @return Success, or Failure if wait or reset returns a non-success VkResult.
	 */
    clz::Result waitForGPU(const types::Device device, VkFence *pFence)
    {
        if (vkWaitForFences(device.device, 1, pFence, VK_TRUE, UINT64_MAX) != VK_SUCCESS)
            return clz::Failure("Failed to wait for fence");

        if (vkResetFences(device.device, 1, pFence) != VK_SUCCESS)
            return clz::Failure("Failed to reset fence");

        return clz::Success();
    }

	/**
	 * @brief Acquires the next presentable swapchain image.
	 *
	 * VK_SUBOPTIMAL_KHR is treated as success (image is still usable).
	 * VK_ERROR_OUT_OF_DATE_KHR means the swapchain must be rebuilt — returned
	 * as Failure. Swapchain rebuild is not yet implemented.
	 *
	 * @param device       Logical device.
	 * @param swapchain    Swapchain to acquire from.
	 * @param semaphore    Signaled when the image is safe to render into.
	 * @param pImageIndex  Output: acquired image index.
	 * @return Success (including suboptimal), or Failure with a reason string.
	 */
    clz::Result acquireNextImage(const types::Device device, const types::Swapchain swapchain, VkSemaphore semaphore, uint32_t *pImageIndex)
    {
        VkResult acquireResult = vkAcquireNextImageKHR(device.device, swapchain.swapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, pImageIndex);

        if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
            return clz::Failure("renderer/mainloop: Swapchain is out of date");

        if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
            return clz::Failure("renderer/mainloop: Failed to acquire next image");

        return clz::Success();
    }

	/**
	 * @brief Resets and begins a command buffer for this frame's recording.
	 *
	 * Reset flags are 0 — the pool was created with
	 * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT so this is valid.
	 * BeginInfo has no flags set (not ONE_TIME_SUBMIT — the buffer is reused).
	 *
	 * @param device        Logical device.
	 * @param commandBuffer Command buffer to reset and open for recording.
	 * @return Success, or Failure if either Vulkan call fails.
	 */
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

	/**
	 * @brief Records the full draw sequence for one frame.
	 *
	 * Uses dynamic rendering (VK_KHR_dynamic_rendering / Vulkan 1.3) —
	 * no render pass object. Sequence:
	 *
	 *   1. Barrier: UNDEFINED → COLOR_ATTACHMENT_OPTIMAL
	 *      (src: TOP_OF_PIPE / no access, dst: COLOR_ATTACHMENT_OUTPUT / write)
	 *   2. vkCmdBeginRendering with a black clear, store op, single color attachment
	 *   3. Dynamic viewport + scissor set to swapchain extent
	 *   4. Bind graphics pipeline
	 *   5. vkCmdDraw(3, 1, 0, 0) — hardcoded triangle, positions in vertex shader
	 *   6. vkCmdEndRendering
	 *   7. Barrier: COLOR_ATTACHMENT_OPTIMAL → PRESENT_SRC_KHR
	 *      (src: COLOR_ATTACHMENT_OUTPUT / write, dst: BOTTOM_OF_PIPE / no access)
	 *   8. vkEndCommandBuffer
	 *
	 * @param commandBuffer Command buffer in recording state.
	 * @param swapchain     Source of images, image views, and extent.
	 * @param imageIndex    Which swapchain image to render into.
	 * @param pipeline      Graphics pipeline to bind.
	 * @return Success, or Failure if vkEndCommandBuffer fails.
	 */
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

        vkCmdBindPipeline(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
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

	/**
	 * @brief Submits the recorded command buffer to the graphics queue.
	 *
	 * Uses vkQueueSubmit2 (sync2 / Vulkan 1.3). Semaphore wait is scoped to
	 * COLOR_ATTACHMENT_OUTPUT so the GPU can process earlier pipeline stages
	 * before the image is available. Signal semaphore scope is ALL_GRAPHICS.
	 *
	 * @param queue                   Graphics queue.
	 * @param commandBuffer           Recorded command buffer to submit.
	 * @param imageAvailableSemaphore Wait semaphore — blocks until image is ready.
	 * @param renderFinishedSemaphore Signal semaphore — fires when rendering is done.
	 * @param inFlightFence           Signal fence — CPU can reuse this frame's resources.
	 * @return Success, or Failure if vkQueueSubmit2 fails.
	 */
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

	/**
	 * @brief Presents the rendered swapchain image to the display.
	 *
	 * Waits on @p semaphore (renderCompleteSemaphore) before presenting.
	 * Uses the graphics queue — assumes it supports presentation, which
	 * holds on most desktop hardware. A dedicated present queue path
	 * is not implemented.
	 *
	 * @param swapchain   Swapchain to present from.
	 * @param queue       Queue to submit the present operation to.
	 * @param semaphore   Wait semaphore — render must be complete.
	 * @param imageIndex  Swapchain image index to present.
	 * @return Success, or Failure if vkQueuePresentKHR fails.
	 */
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
