/**
 * @file cleaners.hpp
 * @brief Vulkan resource destruction functions for the renderer subsystem.
 *
 * Each function is the counterpart of a creation function in any of the files.
 * Consider this as a global cleaners file
 * Call order matters — destroy in reverse init order.
 * renderer.cpp calls vkDeviceWaitIdle() before any of these.
 */

#pragma once

#include "render_types.hpp"

namespace clz
{
	namespace render
	{
		/**
		 * @brief Destroys the VkInstance.
		 *
		 * Must be called last — after surface, validation layers, and device
		 * are already destroyed.
		 *
		 * @param pInstance Non-null pointer to the Instance to destroy.
		 */
		void destroyInstance(types::Instance *pInstance);

		/**
		 * @brief Destroys the debug utils messenger.
		 *
		 * Uses vkb::destroy_debug_utils_messenger. Safe to call even if
		 * the messenger is VK_NULL_HANDLE (release builds).
		 *
		 * @param pValidationLayers Non-null pointer to the ValidationLayers to destroy.
		 * @param instance          The instance the messenger was created against.
		 */
		void destroyValidationLayers(types::ValidationLayers *pValidationLayers, const types::Instance &instance);

		/**
		 * @brief Destroys the VkSurfaceKHR.
		 *
		 * Must be called before destroyInstance().
		 *
		 * @param pSurface  Non-null pointer to the Surface to destroy.
		 * @param instance  The instance the surface was created against.
		 */
		void destroySurface(types::Surface *pSurface, const types::Instance &instance);

		/**
		 * @brief Destroys the logical device.
		 *
		 * Must be called after all device-owned resources (swapchain, pipeline,
		 * command pool, sync primitives) are destroyed.
		 *
		 * @param pDevice Non-null pointer to the Device to destroy.
		 */
		void destroyDevice(types::Device *device);

		/**
		 * @brief Destroys all swapchain image views and the swapchain itself.
		 *
		 * Image views are destroyed first since they reference the swapchain images.
		 * The swapchain images themselves are owned by the swapchain and are freed
		 * automatically.
		 *
		 * @param device    Logical device.
		 * @param swapchain Non-null pointer to the Swapchain to destroy.
		 */
		void destroySwapchain(const types::Device &device, types::Swapchain *swapchain);

		/**
		 * @brief Destroys the graphics pipeline, pipeline layout, and shader modules.
		 *
		 * Shader modules are destroyed here even though they could have been
		 * destroyed right after pipeline creation — keeping them alive until
		 * explicit cleanup is fine and keeps ownership simple.
		 *
		 * @param device        Logical device.
		 * @param pPipeline     Non-null pointer to the Pipeline to destroy.
		 * @param pShaderModules Non-null pointer to the ShaderModules to destroy.
		 */
		void destroyPipeline(const types::Device &device, types::Pipeline *pPipeline, types::ShaderModules *pShaderModules);

		/**
		 * @brief Destroys the command pool (and implicitly all allocated command buffers).
		 *
		 * Destroying the pool frees all command buffers allocated from it.
		 * No need to call vkFreeCommandBuffers beforehand.
		 *
		 * @param device       Logical device.
		 * @param pCommandPool Non-null pointer to the CommandPool to destroy.
		 */
		void destroyCommandPool(const types::Device &device, types::CommandPool *pCommandPool);

		/**
		 * @brief Destroys all semaphores and fences in the Syncers struct.
		 *
		 * Iterates all three vectors (imageAvailableSemaphores,
		 * renderCompleteSemaphores, inFlightFence) and destroys each handle.
		 *
		 * @param device   Logical device.
		 * @param pSyncers Non-null pointer to the Syncers to destroy.
		 */
		void destroySyncers(const types::Device &device, types::Syncers *pSyncers);
	}
}
