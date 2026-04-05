/*
 * Main Window file
 * If you need anything from window...
 * Just include this file
 */
#pragma once

#include <vector>
#include <cstdint>
#include "clz_types.hpp"
#include <vulkan/vulkan.h>

namespace clz
{
namespace window
{
	// Native window stuff (native.cpp)
	clz::Result init();
	clz::Result update(float deltaTime);
	bool windowShouldClose();
	clz::Result cleanup();

    // Returns required vulkan instance extensions (render.cpp)
	void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount);
    // creates vulkan surface (render.cpp)
    clz::Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance);


    // returns height and width of framebuffer (window.cpp)
    void getFramebufferExtents(int *pWidth, int *pHeight);

}
}
