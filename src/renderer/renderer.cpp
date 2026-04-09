/**
 * @file renderer.cpp
 * @brief Module-level Vulkan state and public renderer lifecycle implementation.
 *
 * Owns all module-level Vulkan object instances (r_* variables) and
 * implements the three public functions from renderer.hpp.
 * All real work is delegated to initializers.hpp, cleaners.hpp,
 * commandbuffer.hpp, and mainloop.hpp.
 *
 * Frame state:
 *   - r_currentFrame: cycles 0..FRAMES_IN_FLIGHT-1, indexes into per-frame sync primitives.
 *   - r_imageIndex:   the swapchain image index acquired for the current frame.
 */

#include "renderer/renderer.hpp"
#include "clz_types.hpp"
#include "renderer/render_types.hpp"
#include "renderer/initializers.hpp"
#include "renderer/cleaners.hpp"
#include "renderer/commandbuffer.hpp"
#include "renderer/mainloop.hpp"
#include <array>

namespace clz::render
{
	types::Instance           r_instance;        ///< Vulkan instance.
	types::ValidationLayers   r_validationLayers; ///< Debug messenger (null in release).
	types::Surface            r_surface;          ///< Window surface.
	types::PhysicalDevice     r_physicalDevice;   ///< Selected GPU.
	types::Device             r_device;           ///< Logical device.
	types::Queue              r_queue;            ///< Graphics + present queues.
	types::Swapchain          r_swapchain;        ///< Swapchain + images + image views.
	types::ShaderModules      r_shaderModules;    ///< Vertex + fragment SPIR-V modules.
	types::Pipeline           r_Pipeline;         ///< Graphics pipeline + layout.
	types::Syncers            r_syncers;          ///< Semaphores + in-flight fences.
	types::CommandPool        r_commandPool;      ///< Command pool (graphics family, reset-per-buffer).
	std::vector<types::CommandBuffer> r_commandBuffers; ///< One command buffer per frame in flight.

	/// Number of frames to pipeline on the GPU simultaneously.
	constexpr size_t FRAMES_IN_FLIGHT = 2;

	/// Cycles 0..FRAMES_IN_FLIGHT-1. Indexes per-frame sync primitives and command buffers.
	uint32_t r_currentFrame = 0;

	/// Swapchain image index acquired for the current frame by vkAcquireNextImageKHR.
	uint32_t r_imageIndex = 0;
}

namespace clz::render
{
	/**
	 * @brief Brings up the full Vulkan renderer stack.
	 *
	 * Initializes all subsystems in dependency order. Any failure
	 * propagates immediately via INVALIDATE, leaving partial state
	 * uncleaned (full init is a one-time path; partial cleanup is
	 * not implemented).
	 *
	 * Shader paths are hardcoded to:
	 *   assets/shaders/triangle_vert.spirv
	 *   assets/shaders/triangle_frag.spirv
	 *
	 * @return Success if all subsystems came up, Failure otherwise.
	 */
	Result init()
	{
	    // Base initialization
		INVALIDATE(initVulkan(
				&r_instance, &r_validationLayers,
				&r_surface,
				&r_physicalDevice, &r_device,
				&r_queue
		));

		// Swapchain initialization
        INVALIDATE(createSwapchain(r_physicalDevice, r_device, r_surface, r_queue, &r_swapchain));

        // Pipeline initialization
        std::array<std::string, 2> shaderFiles = {"assets/shaders/triangle_vert.spirv", "assets/shaders/triangle_frag.spirv"};
        INVALIDATE(createShaderModules(r_device, shaderFiles, &r_shaderModules));
        INVALIDATE(createPipeline(r_device, r_swapchain, r_shaderModules, &r_Pipeline));

		// Syncers initialization
		INVALIDATE(createSyncers(r_device, r_swapchain, FRAMES_IN_FLIGHT, &r_syncers));

		// Command pool + buffers initialization
		INVALIDATE(createCommandPool(r_device, r_queue, &r_commandPool));
        INVALIDATE(createCommandBuffers(r_device, r_commandPool, &r_commandBuffers, FRAMES_IN_FLIGHT));

        clz::Result result;
        result.success = true;
        return result;
    }

	/**
	 * @brief Renders one frame using the double-buffered frame-in-flight loop.
	 *
	 * All steps are indexed by r_currentFrame except r_imageIndex which comes
	 * from vkAcquireNextImageKHR and may differ from r_currentFrame when the
	 * swapchain has more images than FRAMES_IN_FLIGHT.
	 *
	 * r_currentFrame is advanced at the end with a modulo wrap.
	 *
	 * @param deltaTime Unused. Reserved for future animation/input use.
	 * @return Success each frame, Failure if any Vulkan call fails.
	 */
    clz::Result update(float deltaTime)
    {
        INVALIDATE(waitForGPU(r_device, &r_syncers.inFlightFence[r_currentFrame]));
        INVALIDATE(acquireNextImage(r_device, r_swapchain, r_syncers.imageAvailableSemaphores[r_currentFrame], &r_imageIndex));
        INVALIDATE(startCommandBuffer(r_device, r_commandBuffers[r_currentFrame]));
        INVALIDATE(recordCommandBuffer(r_commandBuffers[r_currentFrame], r_swapchain, r_imageIndex, r_Pipeline));
        INVALIDATE(submitCommandBuffer(r_queue, r_commandBuffers[r_currentFrame], r_syncers.imageAvailableSemaphores[r_currentFrame], r_syncers.renderCompleteSemaphores[r_currentFrame], r_syncers.inFlightFence[r_currentFrame]));
        INVALIDATE(present(r_swapchain, r_queue, r_syncers.renderCompleteSemaphores[r_currentFrame], r_imageIndex));

        r_currentFrame = (r_currentFrame + 1) % FRAMES_IN_FLIGHT;
        return clz::Success();
    }

	/**
	 * @brief Tears down all Vulkan resources in reverse initialization order.
	 *
	 * vkDeviceWaitIdle() is called first to guarantee the GPU is idle
	 * before any handle is destroyed. Destruction order:
	 *   command pool → syncers → pipeline → swapchain →
	 *   device → surface → validation layers → instance
	 */
	void cleanup()
	{
		vkDeviceWaitIdle(r_device.device);
		destroyCommandPool(r_device, &r_commandPool);
		destroySyncers(r_device, &r_syncers);
		destroyPipeline(r_device, &r_Pipeline, &r_shaderModules);
		destroySwapchain(r_device, &r_swapchain);
		destroyDevice(&r_device);
		destroySurface(&r_surface, r_instance);
		destroyValidationLayers(&r_validationLayers, r_instance);
		destroyInstance(&r_instance);
	}
}
