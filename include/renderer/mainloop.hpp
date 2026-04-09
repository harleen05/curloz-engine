/**
 * @file mainloop.hpp
 * @brief Per-frame Vulkan render loop operations for the renderer subsystem.
 *
 * These functions implement the six steps of the double-buffered render loop.
 * They are called in sequence by render::update() in renderer.cpp — not meant
 * to be called directly by external code.
 *
 * Call order per frame:
 *   waitForGPU → acquireNextImage → startCommandBuffer →
 *   recordCommandBuffer → submitCommandBuffer → present
 */

#pragma once

#include "renderer/render_types.hpp"
#include "clz_types.hpp"

namespace clz
{
namespace render
{
	/**
	 * @brief Waits for the in-flight fence and resets it for the next frame.
	 *
	 * Blocks the CPU until the GPU signals the fence, ensuring the command
	 * buffer and sync primitives for this frame index are no longer in use.
	 * Resets the fence after waiting so it can be re-signaled on next submit.
	 *
	 * @param device Logical device.
	 * @param pFence Pointer to the fence to wait on and reset.
	 * @return Success, or Failure if wait or reset fails.
	 */
	clz::Result waitForGPU(const types::Device device, VkFence *pFence);

	/**
	 * @brief Acquires the next available swapchain image index.
	 *
	 * Signals @p semaphore when the image is ready to render into.
	 * Returns Failure on VK_ERROR_OUT_OF_DATE_KHR (swapchain needs rebuild)
	 * or any other error. VK_SUBOPTIMAL_KHR is treated as success.
	 *
	 * @param device       Logical device.
	 * @param swapchain    Swapchain to acquire from.
	 * @param semaphore    Semaphore to signal when the image is available.
	 * @param pImageIndex  Output: index into the swapchain image array.
	 * @return Success, or Failure with a reason string.
	 */
	clz::Result acquireNextImage(const types::Device device, const types::Swapchain swapchain, VkSemaphore semaphore, uint32_t *pImageIndex);

	/**
	 * @brief Resets and begins a command buffer for recording.
	 *
	 * Calls vkResetCommandBuffer() then vkBeginCommandBuffer() with
	 * a default VkCommandBufferBeginInfo (one-time-submit not set —
	 * the buffer is reused every frame).
	 *
	 * @param device        Logical device.
	 * @param commandBuffer Command buffer to reset and begin.
	 * @return Success, or Failure if reset or begin fails.
	 */
	clz::Result startCommandBuffer(const types::Device device, types::CommandBuffer commandBuffer);

	/**
	 * @brief Records the full draw sequence into the command buffer.
	 *
	 * Records in order:
	 *   1. Image layout transition: UNDEFINED → COLOR_ATTACHMENT_OPTIMAL
	 *   2. Begin dynamic rendering with a black clear color
	 *   3. Set dynamic viewport and scissor to swapchain extent
	 *   4. Bind graphics pipeline
	 *   5. Draw 3 vertices (hardcoded triangle, no vertex buffer)
	 *   6. End dynamic rendering
	 *   7. Image layout transition: COLOR_ATTACHMENT_OPTIMAL → PRESENT_SRC_KHR
	 *   8. End command buffer
	 *
	 * @param commandBuffer Command buffer to record into (must be in recording state).
	 * @param swapchain     Provides images, image views, and extent.
	 * @param imageIndex    Index of the swapchain image to render into.
	 * @param pipeline      Graphics pipeline to bind.
	 * @return Success, or Failure if vkEndCommandBuffer fails.
	 */
	clz::Result recordCommandBuffer(const types::CommandBuffer commandBuffer, const types::Swapchain swapchain, uint32_t imageIndex, types::Pipeline pipeline);

	/**
	 * @brief Submits the recorded command buffer to the graphics queue.
	 *
	 * Uses vkQueueSubmit2 (Vulkan 1.3 / sync2). Waits on
	 * @p imageAvailableSemaphore at the color attachment output stage,
	 * signals @p renderFinishedSemaphore and @p inFlightFence on completion.
	 *
	 * @param queue                    Graphics queue to submit to.
	 * @param commandBuffer            Recorded command buffer to submit.
	 * @param imageAvailableSemaphore  Wait semaphore — image ready to render into.
	 * @param renderFinishedSemaphore  Signal semaphore — rendering complete.
	 * @param inFlightFence            Signal fence — CPU can reuse this frame's resources.
	 * @return Success, or Failure if vkQueueSubmit2 fails.
	 */
	clz::Result submitCommandBuffer(types::Queue queue, types::CommandBuffer commandBuffer, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, VkFence inFlightFence);

	/**
	 * @brief Presents the rendered swapchain image to the display.
	 *
	 * Waits on @p semaphore before presenting. Uses the graphics queue
	 * for presentation (assumes graphics == present queue or that the
	 * queue supports both).
	 *
	 * @param swapchain   Swapchain to present from.
	 * @param queue       Queue to submit the present operation to.
	 * @param semaphore   Wait semaphore — render must be complete before present.
	 * @param imageIndex  Index of the swapchain image to present.
	 * @return Success, or Failure if vkQueuePresentKHR fails.
	 */
	clz::Result present(const types::Swapchain swapchain, const types::Queue queue, VkSemaphore semaphore, uint32_t imageIndex);
}
}
