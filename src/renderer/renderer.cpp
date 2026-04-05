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
    types::Instance r_instance;
    types::ValidationLayers r_validationLayers;
    types::Surface r_surface;
    types::PhysicalDevice r_physicalDevice;
    types::Device r_device;
    types::Queue r_queue;
    types::Swapchain r_swapchain;
    types::ShaderModules r_shaderModules;
    types::Pipeline r_Pipeline;
    types::Syncers r_syncers;
    types::CommandPool r_commandPool;
    std::vector<types::CommandBuffer> r_commandBuffers;

    constexpr size_t FRAMES_IN_FLIGHT = 2;
    uint32_t r_currentFrame = 0;
    uint32_t r_imageIndex = 0;
}

namespace clz::render
{
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

		// Command pool initialization
		INVALIDATE(createCommandPool(r_device, r_queue, &r_commandPool));
        INVALIDATE(createCommandBuffers(r_device, r_commandPool, &r_commandBuffers, FRAMES_IN_FLIGHT));

        clz::Result result;
        result.success = true;
        return result;
    }

    clz::Result update(float deltaTime)
    {
        INVALIDATE(waitForGPU(r_device, r_syncers.inFlightFence[r_currentFrame]));
        INVALIDATE(acquireNextImage(r_device, r_swapchain, r_syncers.imageAvailableSemaphores[r_currentFrame], &r_imageIndex));
        INVALIDATE(startCommandBuffer(r_device, r_commandBuffers[r_currentFrame]));
        INVALIDATE(recordCommandBuffer(r_commandBuffers[r_currentFrame], r_swapchain, r_imageIndex, r_Pipeline));
        INVALIDATE(submitCommandBuffer(r_queue, r_commandBuffers[r_currentFrame], r_syncers.imageAvailableSemaphores[r_currentFrame], r_syncers.renderCompleteSemaphores[r_imageIndex], r_syncers.inFlightFence[r_currentFrame]));
        INVALIDATE(present(r_swapchain, r_queue, r_syncers.renderCompleteSemaphores[r_imageIndex], r_imageIndex));

        r_currentFrame = (r_currentFrame + 1) % FRAMES_IN_FLIGHT;
        return clz::Success();
    }

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
