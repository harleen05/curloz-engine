#define GLFW_INCLUDE_VULKAN
#include "window/render.hpp"

namespace clz::window
{
	void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount, const types::Window &window)
	{
		const char **windowExtensions;
		windowExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		for (uint32_t i=0; i<extensionsCount; ++i)
		{
			Extensions.emplace_back(windowExtensions[i]);
		}
	}
	Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance, const types::Window &window)
	{
		clz::Result result;
		if (glfwCreateWindowSurface(instance, window.window, nullptr, surface) != VK_SUCCESS)
		{
			result.message = "src/window/render.cpp:Could not create Window Surface";
			result.success = false;
			return result;
		}
		result.success = true;
		return result;
	}
}
