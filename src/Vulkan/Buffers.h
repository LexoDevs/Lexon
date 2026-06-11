#pragma once

#include "VulkanConstants.h"

#include "Pipeline.h"

class Pool {
public:
    void createCommandPool(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
    void destroyCommandPool(LogicalDevice logicaldevice);
    
    VkCommandPool getCommandPool(){ return commandPool;}

private:
    VkCommandPool commandPool = nullptr;


};

// Agrupacion de vertices temporalmente aqui para la creacion del indexbuffer
class VertexBuffer {
    public:
        void createCommandBuffer(LogicalDevice logicaldevice, Pool pool);
        void recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline, uint32_t currentFrame,VertexBuffer& vertexbuffer);

        void createBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void createVertexBuffer(LogicalDevice logicaldevice,PhysicalDevice physicaldevice, Pool commandPool);
        
        void createIndexBuffer(LogicalDevice logicaldevice,PhysicalDevice physicaldevice, Pool commandPool);


        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,PhysicalDevice physicaldevice);
        void destroyVertexBuffer(LogicalDevice logicaldevice);

        VkBuffer getVertexBuffer(){ return vertexBuffer;}

        void setVertexBuffer(VkBuffer new_vertexbuffer){  vertexBuffer = new_vertexbuffer;}
        VkCommandBuffer& getCommandBuffer(int i){ return commandBuffers[i];};
    private:
        VkBuffer vertexBuffer = nullptr;
        VkDeviceMemory vertexBufferMemory = nullptr;
        VkBuffer indexBuffer = nullptr;
        VkDeviceMemory indexBufferMemory = nullptr;

        VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];

};




