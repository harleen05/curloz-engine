/**
 * @file commandbuffer.hpp
 * @brief Command pool and command buffer allocation for the renderer subsystem.
 */

#pragma once

#include "clz_types.hpp"
#include "render_types.hpp"
#include <cstdint>

namespace clz
{
namespace render
{
	/**
	 * @brief Creates a command pool for the graphics queue family.
	 *
	 * Created with VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT so
	 * individual command buffers can be reset per-frame without resetting
	 * the entire pool.
	 *
	 * @param device       Logical device.
	 * @param queue        Queue whose graphicsFamily index the pool is created for.
	 * @param pCommandPool Output: populated CommandPool.
	 * @return Success, or Failure if vkCreateCommandPool fails.
	 */
	clz::Result createCommandPool(const types::Device &device, const types::Queue &queue, types::CommandPool *pCommandPool);

	/**
	 * @brief Allocates @p count primary command buffers from the given pool.
	 *
	 * Allocated at VK_COMMAND_BUFFER_LEVEL_PRIMARY. The resulting
	 * CommandBuffer wrappers are appended to @p pCommandBuffers.
	 * Freed implicitly when the pool is destroyed.
	 *
	 * @param device          Logical device.
	 * @param commandPool     Pool to allocate from.
	 * @param pCommandBuffers Output vector to append allocated buffers to.
	 * @param count           Number of command buffers to allocate. Defaults to 1.
	 * @return Success, or Failure if vkAllocateCommandBuffers fails.
	 */
	clz::Result createCommandBuffers(const types::Device &device, const types::CommandPool &commandPool, std::vector<types::CommandBuffer> *pCommandBuffers, uint8_t count = 1);
}
}
