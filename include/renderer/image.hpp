#pragma once

#include <vulkan/vulkan.h>

namespace clz
{
namespace render
{
	/**
	 * @brief Inserts a pipeline barrier to transition an image's layout.
	 *
	 * Wraps VkImageMemoryBarrier2 + VkDependencyInfo + vkCmdPipelineBarrier2
	 * (Vulkan 1.3 / synchronization2). Transitions a single-mip, single-layer
	 * color image. Queue family ownership is not transferred
	 * (VK_QUEUE_FAMILY_IGNORED on both sides).
	 *
	 * Common usage in the render loop:
	 *   - UNDEFINED → COLOR_ATTACHMENT_OPTIMAL before rendering
	 *   - COLOR_ATTACHMENT_OPTIMAL → PRESENT_SRC_KHR before presentation
	 *
	 * @param image           Image to transition.
	 * @param oldLayout       Current image layout.
	 * @param newLayout       Target image layout.
	 * @param src_access_mask Access mask for the source stage (what was written).
	 * @param dst_access_mask Access mask for the destination stage (what will read/write).
	 * @param src_stage_mask  Pipeline stage that produced the data.
	 * @param dst_stage_mask  Pipeline stage that will consume the data.
	 * @param commandBuffer   Command buffer to record the barrier into.
	 */
	void transition_image_layout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags2 src_access_mask, VkAccessFlags2 dst_access_mask, VkPipelineStageFlags2 src_stage_mask, VkPipelineStageFlags2 dst_stage_mask, VkCommandBuffer commandBuffer);
}
}
