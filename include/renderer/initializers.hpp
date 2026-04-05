#pragma once

#include "render_types.hpp"
#include "clz_types.hpp"

namespace clz::render
{
	clz::Result initVulkan(types::Instance *pInstance, types::ValidationLayers *pDebugMessenger, types::Surface *pSurface, types::PhysicalDevice *pPhysicalDevice, types::Device *pDevice, types::Queue *pQueue);

    clz::Result createSwapchain(const types::PhysicalDevice &physicalDevice, const types::Device &device, const types::Surface &surface, const types::Queue &queue, types::Swapchain *pSwapchain);

    clz::Result createShaderModules(const types::Device &device, const std::array<std::string, 2> &shaderFiles, types::ShaderModules *pShaderModules);

    clz::Result createPipeline(const types::Device &device, const types::Swapchain &swapchain, types::ShaderModules shaderModules,types::Pipeline *pPipeline);

    clz::Result createSyncers(const types::Device &device, const types::Swapchain &swapchain, const uint8_t FRAMES_IN_FLIGHT, types::Syncers *pSyncers);
}
