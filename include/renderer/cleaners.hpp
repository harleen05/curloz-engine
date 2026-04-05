#pragma once

#include "render_types.hpp"

namespace clz
{
	namespace render
	{
		void destroyInstance(types::Instance *pInstance);
		void destroyValidationLayers(types::ValidationLayers *pValidationLayers, const types::Instance &instance);
		void destroySurface(types::Surface *pSurface, const types::Instance &instance);
		void destroyDevice(types::Device *device);
        void destroySwapchain(const types::Device &device, types::Swapchain *swapchain);
        void destroyPipeline(const types::Device &device, types::Pipeline *pPipeline, types::ShaderModules *pShaderModules);
        void destroySyncers(const types::Device &device, types::Syncers *pSyncers);
	}
}
