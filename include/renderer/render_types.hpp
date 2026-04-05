#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>

namespace clz
{
namespace render
{
	namespace types
	{
		struct Instance
		{
			VkInstance instance;
			Instance()
			{
				instance = VK_NULL_HANDLE;
			}
		};

		struct ValidationLayers
		{
			VkDebugUtilsMessengerEXT debugMessenger;
			ValidationLayers()
			{
				debugMessenger = VK_NULL_HANDLE;
			}
		};

		struct Surface
		{
			VkSurfaceKHR surface;
		};

		struct PhysicalDevice
		{
			VkPhysicalDevice physicalDevice;
			PhysicalDevice()
			{
				physicalDevice = VK_NULL_HANDLE;
			}
		};

		struct Device
		{
			VkDevice device;
			Device()
			{
				device = VK_NULL_HANDLE;
			}
		};

		struct Queue
		{
			VkQueue graphicsQueue;
			uint32_t graphicsFamily;
			VkQueue presentQueue;
			uint32_t presentFamily;
			Queue()
			{
				graphicsQueue = VK_NULL_HANDLE;
				graphicsFamily = UINT32_MAX;
				presentQueue = VK_NULL_HANDLE;
				presentFamily = UINT32_MAX;
			}
		};

        struct Swapchain
        {
                VkSwapchainKHR swapchain;
                VkSurfaceFormatKHR format;
                VkPresentModeKHR presentMode;
                VkExtent2D extent;
                std::vector<VkImage> swapchainImages;
                std::vector<VkImageView> swapchainImageViews;

                Swapchain()
                {
                        swapchain = VK_NULL_HANDLE;
                }
        };

        struct ShaderModules
        {
            VkShaderModule vertexShaderModule;
            VkShaderModule fragShaderModule;
            ShaderModules()
            {
                vertexShaderModule = VK_NULL_HANDLE;
                fragShaderModule = VK_NULL_HANDLE;
            }
        };
        struct Pipeline
        {
                VkPipeline pipeline;
                VkPipelineLayout pipelineLayout;

                Pipeline()
                {
                        pipeline = VK_NULL_HANDLE;
                        pipelineLayout = VK_NULL_HANDLE;
                }
        };

        struct CommandPool
        {
            VkCommandPool commandPool;
            CommandPool()
            {
                commandPool = VK_NULL_HANDLE;
            }
        };

        struct CommandBuffer
        {
            VkCommandBuffer commandBuffer;
            CommandBuffer()
            {
                commandBuffer = VK_NULL_HANDLE;
            }
        };

        struct Syncers
        {
            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderCompleteSemaphores;
            std::vector<VkFence> inFlightFence;

            Syncers()
            {
                imageAvailableSemaphores.resize(0);
                renderCompleteSemaphores.resize(0);
                inFlightFence.resize(0);
            }
        };
	}
}
}
