#pragma once

#include <vector>
#include <cstdint>
#include "clz_types.hpp"
#include <vulkan/vulkan.h>
#include "window/window_types.hpp"

namespace clz
{
	namespace window
	{
		// Render.cpp
		void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount, const types::Window &window);
		Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance, const types::Window &window);
	}
}
