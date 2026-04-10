/**
 * @file initializers.hpp
 * @brief Vulkan resource creation functions for the renderer subsystem.
 *
 * All functions follow the same convention: inputs are const refs,
 * outputs are raw pointers prefixed with p. On failure, returns a
 * Failure result with a location-prefixed message. On success, returns
 * clz::Success(). None of these functions clean up on partial failure —
 * callers are responsible for cleanup if INVALIDATE triggers a bail-out.
 */

#pragma once

#include "render_types.hpp"
#include "clz_types.hpp"

namespace clz::render
{
	/**
	 * @brief Initializes the core Vulkan objects needed before any rendering.
	 *
	 * Creates in order: instance (+ validation layers if debug), window surface,
	 * physical device selection (requires Vulkan 1.3 + sync2 + dynamicRendering),
	 * logical device, and graphics/present queues.
	 *
	 * Uses vkb (VkBootstrap) internally for instance and device setup.
	 *
	 * @param pInstance         Output: populated VkInstance wrapper.
	 * @param pDebugMessenger   Output: populated debug messenger (null in release).
	 * @param pSurface          Output: populated VkSurfaceKHR wrapper.
	 * @param pPhysicalDevice   Output: selected physical device wrapper.
	 * @param pDevice           Output: created logical device wrapper.
	 * @param pQueue            Output: graphics + present queue handles and family indices.
	 * @return Success, or Failure with an error message on any step failing.
	 */
	clz::Result initVulkan(types::Instance *pInstance, types::ValidationLayers *pDebugMessenger, types::Surface *pSurface, types::PhysicalDevice *pPhysicalDevice, types::Device *pDevice, types::Queue *pQueue);

	/**
	 * @brief Creates the swapchain and all associated image views.
	 *
	 * Queries surface capabilities, picks the best available format
	 * (prefers B8G8R8A8_SRGB / SRGB_NONLINEAR) and present mode
	 * (prefers MAILBOX, falls back to FIFO). Handles both cases where
	 * currentExtent is defined and where it must be clamped from the
	 * framebuffer size. Creates one image view per swapchain image.
	 *
	 * @param physicalDevice Physical device for capability queries.
	 * @param device         Logical device for swapchain and image view creation.
	 * @param surface        Window surface to create the swapchain against.
	 * @param queue          Queue info for sharing mode selection.
	 * @param pSwapchain     Output: fully populated Swapchain struct.
	 * @return Success, or Failure with a message on any step failing.
	 */
	clz::Result createSwapchain(const types::PhysicalDevice &physicalDevice, const types::Device &device, const types::Surface &surface, const types::Queue &queue, types::Swapchain *pSwapchain);

	/**
	 * @brief Loads SPIR-V shader files from disk and creates VkShaderModules.
	 *
	 * Reads both files in binary mode, creates a VkShaderModule for each.
	 * The modules can be destroyed after the pipeline is built — they are
	 * not needed at draw time.
	 *
	 * @param device       Logical device for shader module creation.
	 * @param shaderFiles  Array of exactly 2 paths: { vertex.spirv, fragment.spirv }.
	 * @param pShaderModules Output: vertex and fragment shader module handles.
	 * @return Success, or Failure if a file can't be opened or a module can't be created.
	 */
	clz::Result createShaderModules(const types::Device &device, const std::array<std::string, 2> &shaderFiles, types::ShaderModules *pShaderModules);

	/**
	 * @brief Creates the graphics pipeline and pipeline layout.
	 *
	 * Hardcoded for a no-vertex-buffer triangle pipeline:
	 * - Dynamic viewport + scissor state
	 * - No vertex input bindings (positions in shader)
	 * - Triangle list topology
	 * - Back-face culling, clockwise winding
	 * - No depth/stencil
	 * - No blending
	 * - Dynamic rendering (no render pass object — uses VkPipelineRenderingCreateInfo)
	 *
	 * @param device        Logical device.
	 * @param swapchain     Used for format and extent in pipeline rendering info.
	 * @param shaderModules Vertex + fragment shader modules to bind.
	 * @param pPipeline     Output: pipeline and pipeline layout handles.
	 * @return Success, or Failure if layout or pipeline creation fails.
	 */
	clz::Result createPipeline(const types::Device &device, const types::Swapchain &swapchain, types::ShaderModules shaderModules, types::Pipeline *pPipeline);

	/**
	 * @brief Creates per-frame semaphores and fences for CPU-GPU synchronization.
	 *
	 * All three sync primitive vectors are sized to FRAMES_IN_FLIGHT:
	 * - imageAvailableSemaphores: signaled by vkAcquireNextImageKHR
	 * - renderCompleteSemaphores: signaled on render completion, waited by present
	 * - inFlightFence: created pre-signaled so the first frame doesn't deadlock
	 *
	 * @param device           Logical device.
	 * @param swapchain        Used to size renderCompleteSemaphores.
	 * @param FRAMES_IN_FLIGHT Number of frames to pipeline.
	 * @param pSyncers         Output: populated Syncers struct.
	 * @return Success, or Failure if any primitive creation fails.
	 */
	clz::Result createSyncers(const types::Device &device, const types::Swapchain &swapchain, const uint8_t FRAMES_IN_FLIGHT, types::Syncers *pSyncers);
}
