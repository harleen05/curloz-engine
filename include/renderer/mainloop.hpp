#pragma once

#include "renderer/render_types.hpp"
#include "clz_types.hpp"

namespace clz
{
namespace render
{
    clz::Result waitForGPU(const types::Device device, VkFence fence);
    clz::Result acquireNextImage(const types::Device device, const types::Swapchain swapchain, VkSemaphore semaphore, uint32_t *pImageIndex);
    clz::Result startCommandBuffer(const types::Device device, types::CommandBuffer commandBuffer);
    clz::Result recordCommandBuffer(const types::CommandBuffer commandBuffer, const types::Swapchain swapchain, uint32_t imageIndex, types::Pipeline pipeline);
    clz::Result submitCommandBuffer(types::Queue queue, types::CommandBuffer commandBuffer, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, VkFence inFlightFence);
    clz::Result present(const types::Swapchain swapchain, const types::Queue queue, VkSemaphore semaphore, uint32_t imageIndex);
}
}
