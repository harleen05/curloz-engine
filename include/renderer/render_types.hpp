/**
 * @file render_types.hpp
 * @brief Vulkan object wrapper types for the Curloz Engine renderer.
 *
 * Each struct is a thin named wrapper around one or more raw Vulkan handles.
 * All handles are default-constructed to VK_NULL_HANDLE so uninitialized
 * state is detectable. Owned and populated by initializers.hpp;
 * destroyed by the matching functions in cleaners.hpp.
 */

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
		/**
		 * @struct Instance
		 * @brief Wraps the VkInstance handle.
		 */
		struct Instance
		{
			VkInstance instance; ///< Vulkan instance. Null until initVulkan().

			Instance()
			{
				instance = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct ValidationLayers
		 * @brief Wraps the debug utils messenger for validation layer output.
		 *
		 * Only active in debug builds (NDEBUG not defined). Set to
		 * VK_NULL_HANDLE in release. Destroyed via vkb::destroy_debug_utils_messenger.
		 */
		struct ValidationLayers
		{
			VkDebugUtilsMessengerEXT debugMessenger; ///< Debug messenger handle. Null in release builds.

			ValidationLayers()
			{
				debugMessenger = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct Surface
		 * @brief Wraps the VkSurfaceKHR created from the GLFW window.
		 *
		 * Must be destroyed before the VkInstance.
		 */
		struct Surface
		{
			VkSurfaceKHR surface; ///< Vulkan window surface. Null until createVkSurface().

			Surface()
			{
				surface = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct PhysicalDevice
		 * @brief Wraps the selected VkPhysicalDevice handle.
		 *
		 * Not destroyed explicitly — physical device lifetime is tied to the instance.
		 */
		struct PhysicalDevice
		{
			VkPhysicalDevice physicalDevice; ///< Selected GPU handle. Null until initVulkan().

			PhysicalDevice()
			{
				physicalDevice = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct Device
		 * @brief Wraps the VkDevice (logical device) handle.
		 */
		struct Device
		{
			VkDevice device; ///< Logical device handle. Null until initVulkan().

			Device()
			{
				device = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct Queue
		 * @brief Holds graphics and present queue handles and their family indices.
		 *
		 * Graphics and present queues may refer to the same underlying queue on
		 * integrated GPUs — check graphicsFamily == presentFamily before assuming
		 * exclusive ownership.
		 */
		struct Queue
		{
			VkQueue graphicsQueue;    ///< Queue used for graphics command submission.
			uint32_t graphicsFamily;  ///< Queue family index for graphicsQueue.
			VkQueue presentQueue;     ///< Queue used for swapchain presentation.
			uint32_t presentFamily;   ///< Queue family index for presentQueue.

			Queue()
			{
				graphicsQueue = VK_NULL_HANDLE;
				graphicsFamily = UINT32_MAX;
				presentQueue = VK_NULL_HANDLE;
				presentFamily = UINT32_MAX;
			}
		};

		/**
		 * @struct Swapchain
		 * @brief Holds the swapchain handle and all associated image/view data.
		 *
		 * swapchainImages are owned by the swapchain and must not be destroyed manually.
		 * swapchainImageViews are created explicitly and must be destroyed before the swapchain.
		 */
		struct Swapchain
		{
			VkSwapchainKHR swapchain;                      ///< Swapchain handle.
			VkSurfaceFormatKHR format;                     ///< Chosen surface format (prefers B8G8R8A8_SRGB).
			VkPresentModeKHR presentMode;                  ///< Chosen present mode (prefers MAILBOX, falls back to FIFO).
			VkExtent2D extent;                             ///< Framebuffer dimensions in pixels.
			std::vector<VkImage> swapchainImages;          ///< Swapchain images. Owned by the swapchain.
			std::vector<VkImageView> swapchainImageViews;  ///< Image views. Must be destroyed before the swapchain.

			Swapchain()
			{
				swapchain = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct ShaderModules
		 * @brief Holds compiled SPIR-V shader module handles for a vertex + fragment pair.
		 *
		 * Modules can be destroyed after the pipeline is created — they are not
		 * needed at draw time. Destroyed in destroyPipeline().
		 */
		struct ShaderModules
		{
			VkShaderModule vertexShaderModule; ///< Compiled vertex shader module.
			VkShaderModule fragShaderModule;   ///< Compiled fragment shader module.

			ShaderModules()
			{
				vertexShaderModule = VK_NULL_HANDLE;
				fragShaderModule = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct Pipeline
		 * @brief Holds the graphics pipeline and its layout.
		 *
		 * The pipeline layout must be destroyed separately from the pipeline.
		 * Both are destroyed together in destroyPipeline().
		 */
		struct Pipeline
		{
			VkPipeline pipeline;             ///< Graphics pipeline handle.
			VkPipelineLayout pipelineLayout; ///< Pipeline layout handle.

			Pipeline()
			{
				pipeline = VK_NULL_HANDLE;
				pipelineLayout = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct CommandPool
		 * @brief Wraps the VkCommandPool used to allocate command buffers.
		 *
		 * Created with VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT so
		 * individual command buffers can be reset without resetting the whole pool.
		 */
		struct CommandPool
		{
			VkCommandPool commandPool; ///< Command pool handle.

			CommandPool()
			{
				commandPool = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct CommandBuffer
		 * @brief Wraps a single VkCommandBuffer.
		 *
		 * Allocated from a CommandPool. Freed implicitly when the pool is destroyed.
		 */
		struct CommandBuffer
		{
			VkCommandBuffer commandBuffer; ///< Command buffer handle.

			CommandBuffer()
			{
				commandBuffer = VK_NULL_HANDLE;
			}
		};

		/**
		 * @struct Syncers
		 * @brief Holds all per-frame synchronization primitives.
		 *
		 * All three vectors are sized to FRAMES_IN_FLIGHT.
		 * - imageAvailableSemaphores: signaled when a swapchain image is ready to render into.
		 * - renderCompleteSemaphores: signaled when rendering is done, waited on by present.
		 * - inFlightFence: CPU-GPU sync; waited on at the start of each frame to avoid
		 *   overwriting resources still in use by the GPU.
		 */
		struct Syncers
		{
			std::vector<VkSemaphore> imageAvailableSemaphores; ///< One per frame in flight.
			std::vector<VkSemaphore> renderCompleteSemaphores; ///< One per frame in flight.
			std::vector<VkFence> inFlightFence;                ///< One per frame in flight. Created pre-signaled.

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
