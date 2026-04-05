#include "renderer/commandbuffer.hpp"
#include "renderer/render_types.hpp"
#include "vulkan/vulkan_core.h"
#include "clz_types.hpp"

namespace clz
{
namespace render
{

    clz::Result createCommandPool(const types::Device &device, const types::Queue &queue, types::CommandPool *pCommandPool)
    {
        clz::Result result;

        VkCommandPoolCreateInfo commandPoolInfo{};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.queueFamilyIndex = queue.graphicsFamily;
        commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device.device, &commandPoolInfo, nullptr, &pCommandPool->commandPool) != VK_SUCCESS)
        {
            result.message = "src/renderer/initializers.cpp:createCommandPool: Could not create command pool";
            result.success = false;
            return result;
        }

        result.success = true;
        return result;
    }

    clz::Result createCommandBuffers(const types::Device &device, const types::CommandPool &commandPool, std::vector<types::CommandBuffer> *pCommandBuffers, uint8_t count = 1)
    {
        clz::Result result;

        VkCommandBufferAllocateInfo allocateInfo = {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = commandPool.commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = count;

        std::vector<VkCommandBuffer> commandBuffers(count);

        if (vkAllocateCommandBuffers(device.device, &allocateInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            result.message = "Failed to allocate command buffer";
            result.success = false;
            return result;
        }

        for (int i = 0; i < count; ++i)
        {
            types::CommandBuffer commandBuffer;
            commandBuffer.commandBuffer = commandBuffers[i];
            pCommandBuffers->push_back(commandBuffer);
        }
        commandBuffers.clear();
        result.success = true;
        return result;
    }
}
}
