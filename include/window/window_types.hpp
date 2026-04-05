#pragma once

#include <GLFW/glfw3.h>

namespace clz
{
namespace window
{
namespace types
{
    struct Window
    {
        GLFWwindow *window;
        Window() : window(nullptr) {}
    };
}
}
}
