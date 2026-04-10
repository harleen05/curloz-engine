/**
 * @file render.hpp
 * @brief Internal Vulkan-surface interface for the window subsystem.
 *
 * Low-level functions that bridge GLFW and Vulkan, operating directly
 * on a types::Window handle. Not part of the public engine API — use
 * window.hpp instead, which wraps these with the module-level instance.
 */

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
		/**
		 * @brief Queries the Vulkan instance extensions required by GLFW.
		 *
		 * Calls glfwGetRequiredInstanceExtensions() and appends each
		 * extension name pointer to @p Extensions.
		 *
		 * @param Extensions      Output vector to append extension strings to.
		 * @param extensionsCount Output count of required extensions.
		 * @param window          The active Window (used to establish GLFW context).
		 */
		void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount, const types::Window &window);

		/**
		 * @brief Creates a Vulkan surface backed by the GLFW window.
		 *
		 * Wraps glfwCreateWindowSurface(). The caller is responsible for
		 * destroying the surface (vkDestroySurfaceKHR) before the instance.
		 *
		 * @param surface  Output pointer to receive the created VkSurfaceKHR.
		 * @param instance The Vulkan instance to create the surface against.
		 * @param window   The active Window whose handle backs the surface.
		 * @return Success if VK_SUCCESS, Failure with a message otherwise.
		 */
		Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance, const types::Window &window);
	}
}
