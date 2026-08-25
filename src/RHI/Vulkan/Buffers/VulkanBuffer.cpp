#include "VulkanBuffer.h"
#include <iostream>


VkCommandBuffer BufferI::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_Context.commandPool;
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(m_Context.device, &allocInfo, &m_Context.commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(m_Context.commandBuffer, &beginInfo);

    return m_Context.commandBuffer;
}

void BufferI::endSingleTimeCommands() {
    vkEndCommandBuffer(m_Context.commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_Context.commandBuffer;

    vkQueueSubmit(m_Context.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_Context.graphicsQueue);

    vkFreeCommandBuffers(m_Context.device, m_Context.commandPool, 1, &m_Context.commandBuffer);
}


void BufferI::createBuffer(VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    VkBuffer& buffer, VkDeviceMemory& bufferMemory){

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_Context.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_Context.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_Context.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_Context.device, buffer, bufferMemory, 0);
}

void BufferI::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands();

}

uint32_t BufferI::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        vkGetPhysicalDeviceMemoryProperties(m_Context.physicalDevice, &m_Context.memProperties);

        for (uint32_t i = 0; i < m_Context.memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (m_Context.memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                m_Context.VulkanMemoryTypeIndex = i;
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
}
