#pragma once
#include "VulkanVertexBuffer.h"    
    
    class IndexBuffer : public VertexBuffer {
        public:
            IndexBuffer(){};
            ~IndexBuffer();

            void createIndexBuffer(CpuModel& mesh, VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);
            void destroyIndexBuffer();

        private:
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

            VkDevice cp_device;
    };
