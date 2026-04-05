#pragma once

#include <vulkan/vulkan.h>

namespace clz
{
namespace render
{
    void transition_image_layout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkAccessFlags2 src_access_mask, VkAccessFlags2 dst_access_mask, VkPipelineStageFlags2 src_stage_mask, VkPipelineStageFlags2 dst_stage_mask, VkCommandBuffer commandBuffer);
}
}
