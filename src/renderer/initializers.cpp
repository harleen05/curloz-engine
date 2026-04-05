#include "renderer/initializers.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include <vulkan/vulkan_core.h>
#include "clz_types.hpp"
#include "renderer/render_types.hpp"
#include "window/window.hpp"
#include <VkBootstrap/VkBootstrap.h>
#include <array>
#include <algorithm>
#include <fstream>

namespace clz::render
{
    // Enable validation layers??
#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
	constexpr bool enableValidationLayers = true;
#endif

}


namespace clz::render
{
	clz::Result initVulkan(types::Instance *pInstance, types::ValidationLayers *pDebugMessenger, types::Surface *pSurface, types::PhysicalDevice *pPhysicalDevice, types::Device *pDevice, types::Queue *pQueue)
	{
		vkb::InstanceBuilder builder;
		clz::Result result;

		// Instance and Validation layers
		std::vector<const char*> requiredExtensions;
		uint32_t requiredExtensionsCount;
		clz::window::getWindowExtensions(requiredExtensions, requiredExtensionsCount);
		auto inst_ret = builder.set_app_name("Curloz Engine")
				       .request_validation_layers(enableValidationLayers)
				       .use_default_debug_messenger()
				       .require_api_version(1, 3, 0)
				       .enable_extensions(requiredExtensions)
				       .build();
		if (!inst_ret)
		{
			result.success = false;
			result.message = inst_ret.error().message();
			return result;
		}
		vkb::Instance vkb_inst = inst_ret.value();
		pInstance->instance = vkb_inst.instance;
            if (enableValidationLayers)
                pDebugMessenger->debugMessenger = vkb_inst.debug_messenger;
            else
                pDebugMessenger->debugMessenger = VK_NULL_HANDLE;


		// Window surface
		INVALIDATE(clz::window::createVkSurface(&pSurface->surface, pInstance->instance));


		// Device
		VkPhysicalDeviceVulkan13Features features{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
			.synchronization2 = true,
			.dynamicRendering = true
		};
		vkb::PhysicalDeviceSelector selector{ vkb_inst };
		vkb::PhysicalDevice vkb_PhysicalDevice = selector
			.set_minimum_version(1, 3)
			.set_required_features_13(features)
			.set_surface(pSurface->surface)
			.select()
			.value();
		if (!vkb_PhysicalDevice)
		{
			result.success = false;
			result.message = inst_ret.error().message();
			return result;
		}

		vkb::DeviceBuilder deviceBuilder{ vkb_PhysicalDevice };
		vkb::Device vkb_Device = deviceBuilder.build().value();
		pPhysicalDevice->physicalDevice = vkb_PhysicalDevice.physical_device;
		pDevice->device = vkb_Device.device;

		// Queue and families
		pQueue->graphicsQueue = vkb_Device.get_queue(vkb::QueueType::graphics).value();
		pQueue->graphicsFamily = vkb_Device.get_queue_index(vkb::QueueType::graphics).value();
		pQueue->presentQueue = vkb_Device.get_queue(vkb::QueueType::present).value();
		pQueue->presentFamily = vkb_Device.get_queue_index(vkb::QueueType::present).value();

		return result;
	}

    clz::Result createSwapchain(const types::PhysicalDevice &physicalDevice, const types::Device &device, const types::Surface &surface, const types::Queue &queue, types::Swapchain *pSwapchain)
    {
        clz::Result result;

        // 1. Surface Capabilities
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.physicalDevice, surface.surface, &capabilities);

        // 1.1 Get image's format
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.physicalDevice, surface.surface, &formatCount, nullptr);
        if (formatCount == 0)
        {
                result.message = "renderer/initializers.cpp: Selected physical device does not have a valid format, exiting...\n";
                result.success = false;
                return result;
        }
        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.physicalDevice, surface.surface, &formatCount, formats.data());


        // 1.2 get presentation modes;
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.physicalDevice, surface.surface, &presentModeCount, nullptr);
        if (presentModeCount == 0)
        {
                result.message = "renderer/initializers.cpp: Selected physical device does not have a valid presentation modes, exiting...\n";
                result.success = false;
                return result;

        }
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.physicalDevice, surface.surface, &presentModeCount, presentModes.data());


        // Choosing the right things for our swapchain

        pSwapchain->format = formats[0];
        for (const auto  &availableFormat : formats)
        {
                if (availableFormat.format == VK_FORMAT_B8G8R8_SRGB &&
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                        pSwapchain->format = availableFormat;
                        break;
                }
        }

        // triple buffering :)))))
        pSwapchain->presentMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto &availablePresentMode : presentModes)
        {
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                        pSwapchain->presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                        break;
                }
        }


        // Extent
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
                pSwapchain->extent = capabilities.currentExtent;
        }
        else
        {
                int width, height;
                clz::window::getFramebufferExtents(&width, &height);

                VkExtent2D actualExtent =
                {
                        static_cast<uint32_t>(width),
                        static_cast<uint32_t>(height)
                };

                pSwapchain->extent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                pSwapchain->extent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);


        }


        // Finally creating the swapchain
        uint32_t imageCount = capabilities.minImageCount + 1;
        if (imageCount > capabilities.maxImageCount && capabilities.maxImageCount > 0)
                imageCount = capabilities.maxImageCount;

        imageCount = 8;

        VkSwapchainCreateInfoKHR swapchainInfo = {};
        swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainInfo.surface = surface.surface;
        swapchainInfo.minImageCount = imageCount;
        swapchainInfo.imageFormat = pSwapchain->format.format;
        swapchainInfo.imageColorSpace = pSwapchain->format.colorSpace;
        swapchainInfo.imageExtent = pSwapchain->extent;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (queue.graphicsFamily != queue.presentFamily)
        {
                swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainInfo.queueFamilyIndexCount = 2;
                uint32_t QueueFamilyIndices[] = {queue.presentFamily, queue.graphicsFamily};
                swapchainInfo.pQueueFamilyIndices = QueueFamilyIndices;
        }
        else
        {
                swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                swapchainInfo.queueFamilyIndexCount = 0;
                swapchainInfo.pQueueFamilyIndices = nullptr;
        }
        swapchainInfo.preTransform = capabilities.currentTransform;
        swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainInfo.presentMode = pSwapchain->presentMode;
        swapchainInfo.clipped = VK_TRUE;
        swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device.device, &swapchainInfo, nullptr, &pSwapchain->swapchain) != VK_SUCCESS)
        {
                result.success = false;
                result.message = "renderer/initializers.cpp: Could not create swapchain";
                return result;
        }

        vkGetSwapchainImagesKHR(device.device, pSwapchain->swapchain, &imageCount, nullptr);
        pSwapchain->swapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device.device, pSwapchain->swapchain, &imageCount, pSwapchain->swapchainImages.data());


        // retrieving image views
        pSwapchain->swapchainImageViews.resize(pSwapchain->swapchainImages.size());
        for (int i=0; i<pSwapchain->swapchainImages.size(); ++i)
        {
                VkImageViewCreateInfo imageViewInfo = {};
                imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                imageViewInfo.image = pSwapchain->swapchainImages[i];
                imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                imageViewInfo.format = pSwapchain->format.format;
                imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
                imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewInfo.subresourceRange.baseMipLevel = 0;
                imageViewInfo.subresourceRange.levelCount = 1;
                imageViewInfo.subresourceRange.baseArrayLayer = 0;
                imageViewInfo.subresourceRange.layerCount = 1;

                if (vkCreateImageView(device.device, &imageViewInfo, nullptr, &pSwapchain->swapchainImageViews[i]) != VK_SUCCESS)
                {
                        result.message = "renderer/initializers.cpp: Could not create image views";
                        result.success = false;
                        return result;
                }
        }

        result.success = true;
        return result;

    }

    clz::Result createShaderModules(const types::Device &device, const std::array<std::string, 2> &shaderFiles, types::ShaderModules *pShaderModules)
    {

        // Loading Shaders
        std::ifstream vertex_file(shaderFiles[0], std::ios::ate | std::ios::binary);
        std::ifstream frag_file(shaderFiles[1], std::ios::ate | std::ios::binary);
        if (!vertex_file)
        {
                return clz::Failure("renderer/initializers.cpp:createPipeline:Could not open file: " + shaderFiles[0]);
        }
        if (!frag_file)
        {
                return clz::Failure("renderer/initializers.cpp:createPipeline:Could not open file: " + shaderFiles[1]);
        }

        size_t vertexFileSize = (size_t) vertex_file.tellg();
        std::vector<char> vertexShaderCode(vertexFileSize);
        vertex_file.seekg(0);
        vertex_file.read(vertexShaderCode.data(), vertexFileSize);
        vertex_file.close();

        size_t fragFileSize = (size_t) frag_file.tellg();
        std::vector<char> fragShaderCode(fragFileSize);
        frag_file.seekg(0);
        frag_file.read(fragShaderCode.data(), fragFileSize);
        frag_file.close();



        // Creating Shader Module

        VkShaderModuleCreateInfo vertInfo = {};
        vertInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertInfo.codeSize = vertexShaderCode.size();
        vertInfo.pCode = reinterpret_cast<const uint32_t *>(vertexShaderCode.data());
        if (vkCreateShaderModule(device.device, &vertInfo, nullptr, &pShaderModules->vertexShaderModule) != VK_SUCCESS)
        {
                return clz::Failure("render/initializers.cpp:createPipeline: Could not create vertex shader module");
        }

        VkShaderModuleCreateInfo fragInfo = {};
        fragInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragInfo.codeSize = fragShaderCode.size();
        fragInfo.pCode = reinterpret_cast<const uint32_t *>(fragShaderCode.data());
        if (vkCreateShaderModule(device.device, &fragInfo, nullptr, &pShaderModules->fragShaderModule) != VK_SUCCESS)
        {
                return clz::Failure("render/initializers.cpp:createPipeline: Could not create fragment shader module");
        }

        return clz::Success();
    }

    clz::Result createPipeline(const types::Device &device, const types::Swapchain &swapchain, types::ShaderModules shaderModules,types::Pipeline *pPipeline)
    {
	    // Shader Create Info

	    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	    vertShaderStageInfo.module = shaderModules.vertexShaderModule;
	    vertShaderStageInfo.pName = "main";

	    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	    fragShaderStageInfo.module = shaderModules.fragShaderModule;
	    fragShaderStageInfo.pName = "main";

	    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};


	    // Pipeline creation

	    std::vector<VkDynamicState> dynamicStates = {
	    	VK_DYNAMIC_STATE_VIEWPORT,
	    	VK_DYNAMIC_STATE_SCISSOR
	    };

	    VkPipelineDynamicStateCreateInfo dynamicState{};
	    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	    dynamicState.pDynamicStates = dynamicStates.data();

	    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	    vertexInputInfo.vertexBindingDescriptionCount = 0;
	    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
	    vertexInputInfo.vertexAttributeDescriptionCount = 0;
	    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

	    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	    inputAssembly.primitiveRestartEnable = VK_FALSE;

	    VkViewport viewport{};
	    viewport.x = 0.0f;
	    viewport.y = 0.0f;
	    viewport.width = (float) swapchain.extent.width;
	    viewport.height = (float) swapchain.extent.height;
	    viewport.minDepth = 0.0f;
	    viewport.maxDepth = 1.0f;

	    VkRect2D scissor{};
	    scissor.offset = {0, 0};
	    scissor.extent = swapchain.extent;

	    VkPipelineViewportStateCreateInfo viewportState{};
	    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	    viewportState.viewportCount = 1;
	    viewportState.pViewports = &viewport;
	    viewportState.scissorCount = 1;
	    viewportState.pScissors = &scissor;

	    VkPipelineRasterizationStateCreateInfo rasterizer{};
	    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	    rasterizer.depthClampEnable = VK_FALSE;
	    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	    rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.lineWidth = 1.0f;
	    VkPipelineMultisampleStateCreateInfo multisampling{};
	    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	    multisampling.sampleShadingEnable = VK_FALSE;
	    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	    colorBlendAttachment.blendEnable = VK_FALSE;
	    VkPipelineColorBlendStateCreateInfo colorBlending{};
	    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	    colorBlending.logicOpEnable = VK_FALSE;
	    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	    colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

	    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	    pipelineLayoutInfo.setLayoutCount = 0; // Optional
	    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	    if (vkCreatePipelineLayout(device.device, &pipelineLayoutInfo, nullptr, &pPipeline->pipelineLayout) != VK_SUCCESS)
	    	return clz::Failure("src/renderer/initializers.cpp:createPipeline: Could not create pipeline layout");


	    VkPipelineRenderingCreateInfo pipelineRenderingCI =
	    {
	    	.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
	    	.colorAttachmentCount = 1,
	    	.pColorAttachmentFormats = &swapchain.format.format
	    };

	    VkGraphicsPipelineCreateInfo pipelineInfo{};
	    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	    pipelineInfo.pNext = &pipelineRenderingCI;
	    pipelineInfo.stageCount = 2;
	    pipelineInfo.pStages = shaderStages;
	    pipelineInfo.pVertexInputState = &vertexInputInfo;
	    pipelineInfo.pInputAssemblyState = &inputAssembly;
	    pipelineInfo.pViewportState = &viewportState;
	    pipelineInfo.pRasterizationState = &rasterizer;
	    pipelineInfo.pMultisampleState = &multisampling;
	    pipelineInfo.pDepthStencilState = nullptr; // Optional
	    pipelineInfo.pColorBlendState = &colorBlending;
	    pipelineInfo.pDynamicState = &dynamicState;
	    pipelineInfo.layout = pPipeline->pipelineLayout;
	    pipelineInfo.renderPass = VK_NULL_HANDLE;
	    pipelineInfo.subpass = 0;



	    if (vkCreateGraphicsPipelines(device.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pPipeline->pipeline) != VK_SUCCESS)
	    	return clz::Failure("src/renderer/initializers.cpp:createPipeline: Could not create pipeline");

		return clz::Success();
    }

    clz::Result createSyncers(const types::Device &device, const types::Swapchain &swapchain, const uint8_t FRAMES_IN_FLIGHT, types::Syncers *pSyncers)
    {
        clz::Result result;

        pSyncers->renderCompleteSemaphores.resize(static_cast<size_t>(swapchain.swapchainImages.size()));
        for (size_t i=0; i<swapchain.swapchainImages.size(); ++i)
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            if (vkCreateSemaphore(device.device, &semaphoreInfo, nullptr, &pSyncers->renderCompleteSemaphores[i]) != VK_SUCCESS)
            {
                result.message = "src/renderer/initializers.cpp:createSyncers: Could not create semaphores";
                result.success = false;
                return result;
            }
        }

        //pSyncers->imageAvailableSemaphores.resize(swapchain.swapchainImages.size());
        pSyncers->imageAvailableSemaphores.resize(FRAMES_IN_FLIGHT);
        //for (size_t i=0; i<swapchain.swapchainImages.size(); ++i)
        for (size_t i=0; i<FRAMES_IN_FLIGHT; ++i)
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            if (vkCreateSemaphore(device.device, &semaphoreInfo, nullptr, &pSyncers->imageAvailableSemaphores[i]) != VK_SUCCESS)
            {
                result.message = "src/renderer/initializers.cpp:createSyncers: Could not create semaphores";
                result.success = false;
                return result;
            }
        }

        pSyncers->inFlightFence.resize(static_cast<size_t>(FRAMES_IN_FLIGHT));
        for (size_t i=0; i<FRAMES_IN_FLIGHT; ++i)
        {
            VkFenceCreateInfo fenceInfo = {};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            if (vkCreateFence(device.device, &fenceInfo, nullptr, &pSyncers->inFlightFence[i]) != VK_SUCCESS)
            {
                result.message = "src/renderer/initializers.cpp:createSyncers: Could not create fence";
                result.success = false;
                return result;
            }
        }

        result.success = true;
        return result;
    }

}
