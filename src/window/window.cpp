/**
 * @file window.cpp
 * @brief Stateless public forwarders and module-level window state.
 *
 * Owns the single module-level types::Window instance (w_window) and
 * the default dimensions (w_width, w_height). Every function here is
 * a thin forwarder that injects w_window into the real implementation
 * living in native.cpp or render.cpp.
 *
 * Callers never see w_window — they go through window.hpp, which
 * exposes the same signatures without the window parameter.
 *
 * Forwarding pattern:
 * @code
 *   // Public API (window.hpp)        Real impl (native.hpp / render.hpp)
 *   clz::Result init()          --->  init(&w_window)
 *   clz::Result cleanup()       --->  cleanup(&w_window)
 *   void getWindowExtensions()  --->  getWindowExtensions(..., w_window)
 * @endcode
 */

#define GLFW_INCLUDE_VULKAN

#include "window/window.hpp"
#include "window/render.hpp"
#include "window/native.hpp"
#include "window/window_types.hpp"
#include <GLFW/glfw3.h>
#include <fmt/base.h>

namespace clz::window
{
	/// Module-level GLFW window handle. Populated by init(), destroyed by cleanup().
	types::Window w_window;

	/// Default framebuffer width in pixels.
	uint32_t w_width = 800;

	/// Default framebuffer height in pixels.
	uint32_t w_height = 600;
}

namespace clz::window
{
	/**
	 * @brief Forwards to native::init() with the module-level window.
	 * @return Result from native::init().
	 */
	clz::Result init()
	{
	    return init(&w_window);
	}

	/**
	 * @brief Forwards to native::cleanup() with the module-level window.
	 * @return Result from native::cleanup().
	 */
	clz::Result cleanup()
	{
	    return cleanup(&w_window);
	}

	/**
	 * @brief Forwards to native::update() with the module-level window.
	 * @param deltaTime Elapsed time since the last frame (seconds).
	 * @return Result from native::update().
	 */
	clz::Result update(float deltaTime)
	{
	    return update(&w_window, deltaTime);
	}

	/**
	 * @brief Forwards to native::windowShouldClose() with the module-level window.
	 * @return true if the window has been flagged for closing.
	 */
	bool windowShouldClose()
	{
	    return windowShouldClose(&w_window);
	}

	/**
	 * @brief Forwards to render::getWindowExtensions() with the module-level window.
	 * @param Extensions      Output vector to append required extension names to.
	 * @param extensionsCount Output count of required extensions.
	 */
	void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount)
	{
		return getWindowExtensions(Extensions, extensionsCount, w_window);
	}

	/**
	 * @brief Forwards to render::createVkSurface() with the module-level window.
	 * @param surface  Output pointer to receive the created VkSurfaceKHR.
	 * @param instance The Vulkan instance to create the surface against.
	 * @return Result from render::createVkSurface().
	 */
	Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance)
	{
		return createVkSurface(surface, instance, w_window);
	}

	/**
	 * @brief Retrieves the current framebuffer dimensions in pixels.
	 *
	 * Wraps glfwGetFramebufferSize() directly. Prefer this over the
	 * stored w_width/w_height for swapchain extent, as the framebuffer
	 * size can differ from the window size on high-DPI displays.
	 *
	 * @param pWidth  Output pointer for framebuffer width in pixels.
	 * @param pHeight Output pointer for framebuffer height in pixels.
	 */
	void getFramebufferExtents(int *pWidth, int *pHeight)
	{
		glfwGetFramebufferSize(w_window.window, pWidth, pHeight);
	}
}
