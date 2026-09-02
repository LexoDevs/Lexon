#pragma once
#include "../Helpers/VulkanConstants.h"
#include <iostream>

#include "VulkanBuffer.h"
#include "../../../Assets/Importer/CPUModel.h"

struct VertexRange
{
    uint32_t firstVertex;
    uint32_t vertexCount;
};
    class VertexBuffer {
        public:
            VertexBuffer(){};
            ~VertexBuffer();

            void createVertexBuffer(CpuModel& mesh, VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool,VkQueue graphicsQueue,VkCommandBuffer commandBuffer);
            void destroyVertexBuffer();
            void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDevice device,VkPhysicalDevice physicalDevice,VkBuffer& buffer, VkDeviceMemory& bufferMemory);
            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,VkQueue graphicsQueue,VkCommandBuffer commandBuffer);

            VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device,VkCommandBuffer commandBuffer);
            void endSingleTimeCommands(VkCommandPool commandPool, VkDevice device,VkCommandBuffer commandBuffer,VkQueue graphicsQueue);

                        VkBuffer GetBuffer() { return vertexBuffer;};

        protected:

            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            std::vector<GPUMeshRange> meshRanges;

            VkPhysicalDeviceMemoryProperties memProperties;
            uint32_t VulkanMemoryTypeIndex;

            VkDevice cp_device;
            VkCommandPool cp_commandPool;

    };









