/**
 * @file commandbuffer.cpp
 * @brief Command pool and command buffer allocation implementations.
 */

#include "renderer/commandbuffer.hpp"
#include "renderer/render_types.hpp"
#include "vulkan/vulkan_core.h"
#include "clz_types.hpp"

namespace clz
{
namespace render
{
	/**
	 * @brief Creates a command pool tied to the graphics queue family.
	 *
	 * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT allows individual
	 * command buffers to be reset via vkResetCommandBuffer without needing
	 * to reset the entire pool. This is required for the per-frame reset
	 * in startCommandBuffer().
	 *
	 * @param device       Logical device.
	 * @param queue        Provides graphicsFamily index for pool creation.
	 * @param pCommandPool Output: populated CommandPool.
	 * @return Success, or Failure with a location string if creation fails.
	 */
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

	/**
	 * @brief Allocates primary command buffers from the given pool.
	 *
	 * Allocates @p count VK_COMMAND_BUFFER_LEVEL_PRIMARY buffers in a single
	 * vkAllocateCommandBuffers call, then wraps each in a types::CommandBuffer
	 * and appends it to @p pCommandBuffers. The intermediate raw vector is
	 * cleared after wrapping.
	 *
	 * Command buffers are freed implicitly when the parent pool is destroyed —
	 * no explicit vkFreeCommandBuffers call is needed.
	 *
	 * @param device          Logical device.
	 * @param commandPool     Pool to allocate from.
	 * @param pCommandBuffers Output vector to append the allocated buffers to.
	 * @param count           Number of buffers to allocate. Defaults to 1.
	 * @return Success, or Failure if vkAllocateCommandBuffers fails.
	 */
    clz::Result createCommandBuffers(const types::Device &device, const types::CommandPool &commandPool, std::vector<types::CommandBuffer> *pCommandBuffers, uint8_t count)
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
