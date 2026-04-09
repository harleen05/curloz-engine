/**
 * @file native.cpp
 * @brief GLFW lifecycle implementation for the window subsystem.
 *
 * Real implementations of init / update / cleanup / windowShouldClose.
 * All functions take an explicit types::Window* so they carry no global
 * state themselves — the module-level w_window is owned by window.cpp
 * and injected here via the forwarding layer.
 */

#include "window/native.hpp"
#include "clz_types.hpp"
#include <GLFW/glfw3.h>

namespace clz
{
namespace window
{
    /**
     * @brief Initializes GLFW and creates a Vulkan-compatible window.
     *
     * Sequence:
     *   1. glfwInit() — returns Failure on error.
     *   2. Set GLFW_CLIENT_API to GLFW_NO_API (Vulkan, no GL context).
     *   3. Set GLFW_RESIZABLE to false (fixed 800×600 for now).
     *   4. glfwCreateWindow() — returns Failure if handle is null.
     *
     * @param pWindow Non-null pointer to the Window struct to populate.
     * @return Success with a log message, or Failure with an error location string.
     */
    clz::Result init(types::Window *pWindow)
    {
        if (!glfwInit())
			return clz::Failure("src/window/window.cpp:Could not initialize GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		pWindow->window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
		if (!pWindow->window)
			return clz::Failure("src/window/window.cpp:Could not create window handle");

		return clz::Success("Initialized window");
    }

    /**
     * @brief Destroys the GLFW window and terminates the GLFW library.
     *
     * Always call this after the main loop exits and before process shutdown.
     * Calling cleanup() while the window is still in use is undefined behaviour.
     *
     * @param pWindow Non-null pointer to the Window to tear down.
     * @return Success after cleanup completes.
     */
    clz::Result cleanup(types::Window *pWindow)
    {
        glfwDestroyWindow(pWindow->window);
        glfwTerminate();
        return clz::Success("Destroyed window");
    }

    /**
     * @brief Polls GLFW events and handles Escape-to-close for one frame.
     *
     * Calls glfwPollEvents() to dispatch pending OS events, then checks
     * whether the Escape key is held and flags the window for closing if so.
     *
     * @param pWindow   Non-null pointer to the active Window.
     * @param deltaTime Elapsed time since the last frame (seconds).
     *                  Currently unused; reserved for future input/animation use.
     * @return Always returns Success.
     */
    clz::Result update(types::Window *pWindow, float deltaTime)
    {
        glfwPollEvents();
        if (glfwGetKey(pWindow->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(pWindow->window, true);

        return clz::Success();
    }

    /**
     * @brief Returns whether the window has been flagged for closing.
     *
     * The close flag is set either by the user clicking the window's
     * close button or by Escape being pressed in update().
     *
     * @param pWindow Non-null pointer to the active Window.
     * @return true if glfwWindowShouldClose() is set for this window.
     */
    bool windowShouldClose(types::Window *pWindow)
    {
        return glfwWindowShouldClose(pWindow->window);
    }
}
}
