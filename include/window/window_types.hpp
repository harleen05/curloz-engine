/**
 * @file window_types.hpp
 * @brief Window-related type definitions for Curloz Engine.
 *
 * Provides the Window struct that wraps a raw GLFWwindow pointer.
 * Used internally by the window subsystem; consumers should go through
 * the public window.hpp interface instead.
 */

#pragma once

#include <GLFW/glfw3.h>

namespace clz
{
namespace window
{
namespace types
{
    /**
     * @struct Window
     * @brief Thin wrapper around a GLFWwindow pointer.
     *
     * Exists so the rest of the engine never touches GLFWwindow*
     * directly. Default-constructed with a null handle; call
     * window::init() to populate it.
     */
    struct Window
    {
        GLFWwindow *window; ///< Underlying GLFW window handle. Null until init().

        /**
         * @brief Default-constructs a Window with a null handle.
         */
        Window() : window(nullptr) {}
    };
}
}
}
