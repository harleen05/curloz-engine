#define GLFW_INCLUDE_VULKAN

#include "window/window.hpp"
#include "window/render.hpp"
#include "window/native.hpp"
#include "window/window_types.hpp"
#include <GLFW/glfw3.h>
#include <fmt/base.h>

namespace clz::window
{
	types::Window w_window;
	uint32_t w_width = 800, w_height = 600;
}

namespace clz::window
{
	clz::Result init()
	{
	    return init(&w_window);
	}
	clz::Result cleanup()
	{
	    return cleanup(&w_window);
	}
	clz::Result update(float deltaTime)
	{
	    return update(&w_window, deltaTime);
	}
	bool windowShouldClose()
	{
	    return windowShouldClose(&w_window);
	}

	void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount)
	{
		return getWindowExtensions(Extensions, extensionsCount, w_window);
	}
	Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance)
	{
		return createVkSurface(surface, instance, w_window);
	}

        void getFramebufferExtents(int *pWidth, int *pHeight)
        {
                glfwGetFramebufferSize(w_window.window, pWidth, pHeight);
        }
}
