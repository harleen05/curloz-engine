/**
 * @file window.hpp
 * @brief Public interface for the Curloz Engine window subsystem.
 *
 * One-stop include for anything window-related. Wraps the internal
 * native (GLFW) and render (Vulkan surface) layers behind a clean
 * stateless API — callers never touch the underlying types::Window
 * directly.
 *
 * Implementation is split across three translation units:
 *   - native.cpp  : GLFW lifecycle (init / update / cleanup / shouldClose)
 *   - render.cpp  : Vulkan surface and extension queries
 *   - window.cpp  : Thin forwarders + framebuffer extent query
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
	/**
	 * @brief Initializes GLFW and creates the application window.
	 *
	 * Delegates to native::init(). The window is fixed at 800×600,
	 * non-resizable, with no OpenGL context (Vulkan-only).
	 *
	 * @return Success if GLFW and the window handle were created,
	 *         Failure with a message otherwise.
	 */
	clz::Result init();

	/**
	 * @brief Ticks the window subsystem for one frame.
	 *
	 * Pumps the GLFW event queue and checks for the Escape key.
	 * Delegates to native::update().
	 *
	 * @param deltaTime Elapsed time since the last frame (seconds).
	 *                  Currently unused; reserved for future use.
	 * @return Always returns Success.
	 */
	clz::Result update(float deltaTime);

	/**
	 * @brief Returns whether the window has been requested to close.
	 *
	 * Returns true when the user closes the window or presses Escape.
	 * Delegates to native::windowShouldClose().
	 *
	 * @return true if the engine main loop should exit.
	 */
	bool windowShouldClose();

	/**
	 * @brief Destroys the window and terminates GLFW.
	 *
	 * Delegates to native::cleanup(). Safe to call once after
	 * update() returns.
	 *
	 * @return Success after teardown completes.
	 */
	clz::Result cleanup();

	/**
	 * @brief Queries the Vulkan instance extensions required by GLFW.
	 *
	 * Appends the platform surface extensions to @p Extensions and
	 * sets @p extensionsCount accordingly. Delegates to render.cpp.
	 *
	 * @param Extensions      Output vector to append extension name strings to.
	 * @param extensionsCount Output count of required extensions.
	 */
	void getWindowExtensions(std::vector<const char*> &Extensions, uint32_t &extensionsCount);

	/**
	 * @brief Creates a Vulkan surface for the GLFW window.
	 *
	 * Wraps glfwCreateWindowSurface(). Delegates to render.cpp.
	 *
	 * @param surface  Output pointer to receive the created VkSurfaceKHR.
	 * @param instance The Vulkan instance to create the surface against.
	 * @return Success if the surface was created, Failure otherwise.
	 */
	clz::Result createVkSurface(VkSurfaceKHR *surface, const VkInstance &instance);

	/**
	 * @brief Retrieves the current framebuffer dimensions in pixels.
	 *
	 * Wraps glfwGetFramebufferSize(). Use this for Vulkan swapchain
	 * extent rather than the window size, as they can differ on
	 * high-DPI displays.
	 *
	 * @param pWidth  Output pointer for framebuffer width in pixels.
	 * @param pHeight Output pointer for framebuffer height in pixels.
	 */
	void getFramebufferExtents(int *pWidth, int *pHeight);

}
}
