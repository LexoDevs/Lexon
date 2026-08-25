#pragma once
#include "../Core/VulkanContext.h"

class BufferI {
    public:
    BufferI(VulkanContext& context)
        : m_Context(context)
    {
    }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands();
        void createImage(uint32_t width,
             uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, uint32_t mipLevels);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

        private:
            VulkanContext& m_Context;

};


static void transition_image_layout(
    VkCommandBuffer         commandBuffer,
    VkImage                 &image,
    VkImageLayout           old_layout,
    VkImageLayout           new_layout,
    VkImageAspectFlags      aspectMask,
    VkAccessFlags2          src_access_mask,
    VkAccessFlags2          dst_access_mask,
    VkPipelineStageFlags2   src_stage_mask,
    VkPipelineStageFlags2   dst_stage_mask)
{
    VkImageMemoryBarrier2 barrier{};
    barrier.sType                   = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.srcStageMask            = src_stage_mask;
    barrier.srcAccessMask           = src_access_mask;
    barrier.dstStageMask            = dst_stage_mask;
    barrier.dstAccessMask           = dst_access_mask;
    barrier.oldLayout               = old_layout;
    barrier.newLayout               = new_layout;
    barrier.srcQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                   = image;

    barrier.subresourceRange = {
        .aspectMask     = aspectMask,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1
    };

    VkDependencyInfo dependency_info{};
    dependency_info.sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.imageMemoryBarrierCount = 1;
    dependency_info.pImageMemoryBarriers    = &barrier;

    vkCmdPipelineBarrier2(commandBuffer, &dependency_info);
}