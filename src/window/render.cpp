/**
 * @file render.cpp
 * @brief Vulkan-surface and extension-query implementation for the window subsystem.
 *
 * Bridges GLFW and Vulkan. Like native.cpp, these functions take an
 * explicit types::Window reference and carry no global state — w_window
 * is injected by the forwarding layer in window.cpp.
 */

#define GLFW_INCLUDE_VULKAN
#include "window/render.hpp"

namespace clz::window
{
	/**
	 * @brief Queries the Vulkan instance extensions required by GLFW.
	 *
	 * Calls glfwGetRequiredInstanceExtensions() to get the platform
	 * surface extensions (e.g. VK_KHR_surface + VK_KHR_xcb_surface on
	 * Linux/X11, VK_KHR_wayland_surface on Wayland) and appends each
	 * name pointer to @p Extensions.
	 *
	 * @note The returned pointers are owned by GLFW and remain valid
	 *       until glfwTerminate() is called. Do not free them.
	 *
	 * @param Extensions      Output vector to append extension name strings to.
	 * @param extensionsCount Output count populated by GLFW.
	 * @param window          The active Window (establishes GLFW context).
	 */
	void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount, const types::Window &window)
	{
		const char **windowExtensions;
		windowExtensions = glfwGetRequiredInstanceExtensions(&extensionsCount);
		for (uint32_t i=0; i<extensionsCount; ++i)
		{
			Extensions.emplace_back(windowExtensions[i]);
		}
	}

	/**
	 * @brief Creates a Vulkan surface backed by the GLFW window.
	 *
	 * Wraps glfwCreateWindowSurface(). The caller owns the returned
	 * surface and must destroy it with vkDestroySurfaceKHR() before
	 * destroying the VkInstance.
	 *
	 * @param surface  Output pointer to receive the created VkSurfaceKHR.
	 * @param instance The Vulkan instance to create the surface against.
	 * @param window   The active Window whose GLFWwindow* backs the surface.
	 * @return Success if glfwCreateWindowSurface() returns VK_SUCCESS,
	 *         Failure with a location-prefixed error message otherwise.
	 */
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
