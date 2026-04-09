/**
 * @file renderer.hpp
 * @brief Public interface for the Curloz Engine renderer subsystem.
 *
 * One-stop include for anything render-related. Mirrors the window
 * subsystem pattern — callers never touch Vulkan handles directly.
 * All module-level state and Vulkan objects live in renderer.cpp.
 *
 * Typical call order:
 * @code
 *   render::init();
 *   while (!window::windowShouldClose())
 *       render::update(dt);
 *   render::cleanup();
 * @endcode
 */

#pragma once

#include "clz_types.hpp"

namespace clz
{
	namespace render
	{
		/**
		 * @brief Initializes the full Vulkan renderer stack.
		 *
		 * Brings up subsystems in order:
		 *   1. Vulkan instance + validation layers + surface + device + queues
		 *   2. Swapchain
		 *   3. Shader modules + graphics pipeline
		 *   4. Sync primitives (semaphores + fences)
		 *   5. Command pool + command buffers
		 *
		 * Uses INVALIDATE throughout — any failure propagates immediately.
		 *
		 * @return Success if all subsystems initialized, Failure with a message otherwise.
		 */
		clz::Result init();

		/**
		 * @brief Renders one frame using the double-buffered frame-in-flight loop.
		 *
		 * Per-frame sequence:
		 *   1. Wait for the in-flight fence (CPU-GPU sync)
		 *   2. Acquire next swapchain image
		 *   3. Reset + begin command buffer
		 *   4. Record draw commands (image transitions + dynamic rendering + draw call)
		 *   5. Submit command buffer to graphics queue
		 *   6. Present to swapchain
		 *   7. Advance r_currentFrame
		 *
		 * @param deltaTime Elapsed time since the last frame (seconds). Currently unused.
		 * @return Success each frame, or Failure if any Vulkan call fails.
		 */
		clz::Result update(float deltaTime);

		/**
		 * @brief Tears down all Vulkan resources in reverse initialization order.
		 *
		 * Calls vkDeviceWaitIdle() first to ensure the GPU is idle before any
		 * destruction. Destroys: command pool, syncers, pipeline, swapchain,
		 * device, surface, validation layers, instance.
		 */
		void cleanup();
	}
}
