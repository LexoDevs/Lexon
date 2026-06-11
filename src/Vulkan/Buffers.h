#pragma once

#include "VulkanConstants.h"

#include "Pipeline.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};



class Pool {
public:
    void createCommandPool(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
    void destroyCommandPool(LogicalDevice logicaldevice);
    
    void createDescriptorPool(LogicalDevice logicaldevice);
    
    VkCommandPool getCommandPool(){ return commandPool;}
    VkDescriptorPool getDescriptorPool(){ return descriptorPool;}

private:
    VkCommandPool commandPool = nullptr;
    VkDescriptorPool descriptorPool;


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

        void createUniformBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
        void destroyUniformBuffer(LogicalDevice logicaldevice);
        void updateUniformBuffer(Swapchain swapchain, uint32_t currentImage);

        void createDescriptorSets(LogicalDevice logicaldevice, GraphicsPipeline pipeline, Pool pool);


        VkBuffer getVertexBuffer(){ return vertexBuffer;}

        void setVertexBuffer(VkBuffer new_vertexbuffer){  vertexBuffer = new_vertexbuffer;}
        VkCommandBuffer& getCommandBuffer(int i){ return commandBuffers[i];};
        VkBuffer& getUniformBuffer(int i){ return uniformBuffers[i];};

    private:
        VkBuffer vertexBuffer = nullptr;
        VkDeviceMemory vertexBufferMemory = nullptr;

        VkBuffer indexBuffer = nullptr;
        VkDeviceMemory indexBufferMemory = nullptr;

        VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];

        VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT];
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];

};




