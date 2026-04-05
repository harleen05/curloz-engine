#include "renderer/cleaners.hpp"
#include "renderer/render_types.hpp"
#include <VkBootstrap/VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace clz::render
{
	void destroyInstance(types::Instance *pInstance)
	{
		vkDestroyInstance(pInstance->instance, nullptr);
	}
	void destroyValidationLayers(types::ValidationLayers *pValidationLayers, const types::Instance &instance)
	{
		vkb::destroy_debug_utils_messenger(instance.instance, pValidationLayers->debugMessenger, nullptr);
	}
	void destroySurface(types::Surface *pSurface, const types::Instance &pInstance)
	{
		vkDestroySurfaceKHR(pInstance.instance, pSurface->surface, nullptr);
	}
	void destroyDevice(types::Device *pDevice)
	{
		vkDestroyDevice(pDevice->device, nullptr);
	}

	void destroySwapchain(const types::Device &device, types::Swapchain *pSwapchain)
	{
		for (auto imageView : pSwapchain->swapchainImageViews)
			vkDestroyImageView(device.device, imageView, nullptr);

		vkDestroySwapchainKHR(device.device, pSwapchain->swapchain, nullptr);
	}

	void destroyPipeline(const types::Device &device, types::Pipeline *pPipeline, types::ShaderModules *pShaderModules)
	{
		vkDestroyPipeline(device.device, pPipeline->pipeline, nullptr);
		vkDestroyPipelineLayout(device.device, pPipeline->pipelineLayout, nullptr);
		vkDestroyShaderModule(device.device, pShaderModules->vertexShaderModule, nullptr);
		vkDestroyShaderModule(device.device, pShaderModules->fragShaderModule, nullptr);
	}

	void destroyCommandPool(const types::Device &device, types::CommandPool *pCommandPool)
	{
		vkDestroyCommandPool(device.device, pCommandPool->commandPool, nullptr);
	}

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
