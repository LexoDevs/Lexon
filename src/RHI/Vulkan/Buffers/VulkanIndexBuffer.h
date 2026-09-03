#pragma once
#include "VulkanVertexBuffer.h"    
    
    class IndexBuffer : public VertexBuffer {
        public:
            IndexBuffer(){};
            ~IndexBuffer();

            void createIndexBuffer(CpuModel& mesh, VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);
            void destroyBuffer() override;
            VkBuffer GetBuffer() {return indexBuffer;};

        private:
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;
std::vector<GPUMeshRange> meshRanges;

            //VkDevice cp_device;
            //VkCommandPool cp_commandPool;
    };
