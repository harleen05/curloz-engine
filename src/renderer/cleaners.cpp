/**
 * @file cleaners.cpp
 * @brief Vulkan resource destruction implementations.
 *
 * Each function is the exact counterpart of a creation function in
 * initializers.hpp or commandbuffer.hpp. Call order in renderer.cpp
 * follows strict reverse-init order to avoid use-after-free of
 * dependent handles.
 */

#include "renderer/cleaners.hpp"
#include "renderer/render_types.hpp"
#include <VkBootstrap/VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace clz::render
{
	/**
	 * @brief Destroys the VkInstance.
	 *
	 * Must be the last Vulkan object destroyed. All child objects
	 * (surface, device, etc.) must already be gone before this is called.
	 *
	 * @param pInstance Non-null pointer to the Instance to destroy.
	 */
	void destroyInstance(types::Instance *pInstance)
	{
		vkDestroyInstance(pInstance->instance, nullptr);
	}

	/**
	 * @brief Destroys the debug utils messenger via vkb.
	 *
	 * No-op in release builds where debugMessenger is VK_NULL_HANDLE.
	 * Must be called before destroyInstance().
	 *
	 * @param pValidationLayers Non-null pointer to the ValidationLayers to destroy.
	 * @param instance          Instance the messenger was created against.
	 */
	void destroyValidationLayers(types::ValidationLayers *pValidationLayers, const types::Instance &instance)
	{
		vkb::destroy_debug_utils_messenger(instance.instance, pValidationLayers->debugMessenger, nullptr);
	}

	/**
	 * @brief Destroys the VkSurfaceKHR.
	 *
	 * Must be called before destroyInstance() and after destroyDevice().
	 *
	 * @param pSurface  Non-null pointer to the Surface to destroy.
	 * @param pInstance Instance the surface was created against.
	 */
	void destroySurface(types::Surface *pSurface, const types::Instance &pInstance)
	{
		vkDestroySurfaceKHR(pInstance.instance, pSurface->surface, nullptr);
	}

	/**
	 * @brief Destroys the logical device.
	 *
	 * Implicitly waits for all device operations to complete (equivalent
	 * to vkDeviceWaitIdle). In practice, renderer.cpp calls vkDeviceWaitIdle
	 * explicitly before cleanup to be safe.
	 *
	 * @param pDevice Non-null pointer to the Device to destroy.
	 */
	void destroyDevice(types::Device *pDevice)
	{
		vkDestroyDevice(pDevice->device, nullptr);
	}

	/**
	 * @brief Destroys all swapchain image views, then the swapchain.
	 *
	 * Image views must be destroyed before the swapchain since they
	 * reference the swapchain's VkImages. The images themselves are
	 * owned by the swapchain and freed automatically.
	 *
	 * @param device    Logical device.
	 * @param pSwapchain Non-null pointer to the Swapchain to destroy.
	 */
	void destroySwapchain(const types::Device &device, types::Swapchain *pSwapchain)
	{
		for (auto imageView : pSwapchain->swapchainImageViews)
			vkDestroyImageView(device.device, imageView, nullptr);

		vkDestroySwapchainKHR(device.device, pSwapchain->swapchain, nullptr);
	}

	/**
	 * @brief Destroys the graphics pipeline, layout, and both shader modules.
	 *
	 * Shader modules are kept alive until explicit cleanup for simplicity —
	 * they could technically be destroyed right after pipeline creation.
	 *
	 * @param device         Logical device.
	 * @param pPipeline      Non-null pointer to the Pipeline to destroy.
	 * @param pShaderModules Non-null pointer to the ShaderModules to destroy.
	 */
	void destroyPipeline(const types::Device &device, types::Pipeline *pPipeline, types::ShaderModules *pShaderModules)
	{
		vkDestroyPipeline(device.device, pPipeline->pipeline, nullptr);
		vkDestroyPipelineLayout(device.device, pPipeline->pipelineLayout, nullptr);
		vkDestroyShaderModule(device.device, pShaderModules->vertexShaderModule, nullptr);
		vkDestroyShaderModule(device.device, pShaderModules->fragShaderModule, nullptr);
	}

	/**
	 * @brief Destroys the command pool.
	 *
	 * Destroying the pool implicitly frees all command buffers allocated
	 * from it — no need to call vkFreeCommandBuffers first.
	 *
	 * @param device       Logical device.
	 * @param pCommandPool Non-null pointer to the CommandPool to destroy.
	 */
	void destroyCommandPool(const types::Device &device, types::CommandPool *pCommandPool)
	{
		vkDestroyCommandPool(device.device, pCommandPool->commandPool, nullptr);
	}

	/**
	 * @brief Destroys all semaphores and fences in the Syncers struct.
	 *
	 * Iterates all three vectors in order:
	 *   imageAvailableSemaphores → renderCompleteSemaphores → inFlightFence
	 *
	 * @param device   Logical device.
	 * @param pSyncers Non-null pointer to the Syncers to destroy.
	 */
	void destroySyncers(const types::Device &device, types::Syncers *pSyncers)
	{
	    for (auto semaphore : pSyncers->imageAvailableSemaphores)
	        vkDestroySemaphore(device.device, semaphore, nullptr);
	    for (auto semaphore : pSyncers->renderCompleteSemaphores)
	        vkDestroySemaphore(device.device, semaphore, nullptr);
	    for (auto fence : pSyncers->inFlightFence)
	        vkDestroyFence(device.device, fence, nullptr);
	}
}
