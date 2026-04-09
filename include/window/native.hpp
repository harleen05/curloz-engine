/**
 * @file native.hpp
 * @brief Internal GLFW lifecycle interface for the window subsystem.
 *
 * Low-level functions that operate directly on a types::Window handle.
 * Not part of the public engine API — use window.hpp instead, which
 * manages the module-level Window instance and forwards here.
 */

#pragma once

#include "window_types.hpp"
#include "clz_types.hpp"

namespace clz
{
namespace window
{
    /**
     * @brief Initializes GLFW and creates a window into @p pWindow.
     *
     * Calls glfwInit(), applies Vulkan/no-resize hints, and creates
     * an 800×600 window. On any failure returns a Failure result with
     * a descriptive message.
     *
     * @param pWindow Non-null pointer to the Window struct to populate.
     * @return Success with a log message, or Failure on GLFW errors.
     */
    clz::Result init(types::Window *pWindow);

    /**
     * @brief Polls GLFW events and handles Escape-to-close for one frame.
     *
     * @param pWindow   Non-null pointer to the active Window.
     * @param deltaTime Elapsed time since the last frame (seconds).
     *                  Currently unused; reserved for future use.
     * @return Always returns Success.
     */
    clz::Result update(types::Window *pWindow, float deltaTime);

    /**
     * @brief Returns whether the window has been flagged for closing.
     *
     * @param pWindow Non-null pointer to the active Window.
     * @return true if glfwWindowShouldClose() is set for this window.
     */
    bool windowShouldClose(types::Window *pWindow);

    /**
     * @brief Destroys the GLFW window and terminates the GLFW library.
     *
     * @param pWindow Non-null pointer to the Window to tear down.
     * @return Success after cleanup completes.
     */
    clz::Result cleanup(types::Window *pWindow);
}
}
