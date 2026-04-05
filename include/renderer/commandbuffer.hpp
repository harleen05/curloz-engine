#pragma once

#include "clz_types.hpp"
#include "render_types.hpp"
#include <cstdint>

namespace clz
{
namespace render
{
    clz::Result createCommandPool(const types::Device &device, const types::Queue &queue, types::CommandPool *pCommandPool);
    void destroyCommandPool(const types::Device &device, types::CommandPool *pCommandPool);
    clz::Result createCommandBuffers(const types::Device &device, const types::CommandPool &commandPool, std::vector<types::CommandBuffer> *pCommandBuffers, uint8_t count);
}
}
