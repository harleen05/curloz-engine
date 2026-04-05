#include "window/native.hpp"
#include "clz_types.hpp"
#include <GLFW/glfw3.h>

namespace clz
{
namespace window
{
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

    clz::Result cleanup(types::Window *pWindow)
    {
        glfwDestroyWindow(pWindow->window);
        glfwTerminate();
        return clz::Success("Destroyed window");
    }

    clz::Result update(types::Window *pWindow, float deltaTime)
    {
        glfwPollEvents();
        if (glfwGetKey(pWindow->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(pWindow->window, true);

        return clz::Success();
    }

    bool windowShouldClose(types::Window *pWindow)
    {
        return glfwWindowShouldClose(pWindow->window);
    }
}
}
