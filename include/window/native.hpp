#pragma once

#include "window_types.hpp"
#include "clz_types.hpp"

namespace clz
{
namespace window
{
    clz::Result init(types::Window *pWindow);
    clz::Result update(types::Window *pWindow, float deltaTime);
    bool windowShouldClose(types::Window *pWindow);
    clz::Result cleanup(types::Window *pWindow);
}
}
