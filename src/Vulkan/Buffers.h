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
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,PhysicalDevice physicaldevice);

        void createBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        void createCommandBuffer(LogicalDevice logicaldevice, Pool pool);

        void createVertexBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool commandPool, VkImage texture);
        void destroyVertexBuffer(LogicalDevice logicaldevice);

        void createIndexBuffer(LogicalDevice logicaldevice,PhysicalDevice physicaldevice, Pool commandPool, VkImage texture);

        void createUniformBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
        void destroyUniformBuffer(LogicalDevice logicaldevice);
        void updateUniformBuffer(Swapchain swapchain, uint32_t currentImage);

        void setVertexBuffer(VkBuffer new_vertexbuffer){  vertexBuffer = new_vertexbuffer;}

        VkBuffer getVertexBuffer(){ return vertexBuffer;}
        VkBuffer getIndexBuffer(){ return indexBuffer;}

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


};


class Texture {
    public:
        void createTextureImage(VertexBuffer buffer, LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool pool);
        void createImage(LogicalDevice logicaldevice, VertexBuffer buffer, PhysicalDevice physicaldevice, uint32_t width,
             uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void transitionImageLayout(Pool pool, LogicalDevice logicaldevice, VkImage &image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        VkCommandBuffer beginSingleTimeCommands(Pool pool, LogicalDevice logicaldevice);
        void endSingleTimeCommands(VkCommandBuffer commandBuffer, LogicalDevice logicaldevice, Pool pool);
        void copyBufferToImage(Pool pool, LogicalDevice logicaldevice, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void destroyImageTexture(LogicalDevice logicaldevice);
        void createTextureImageView(LogicalDevice logicaldevice, VkImageAspectFlags aspectflags);
        void destroyImageTextureView(LogicalDevice logicaldevice);

        VkImageView createImageView(LogicalDevice logicaldevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        void createImageViews(Swapchain swapchain, LogicalDevice logicaldevice, VkImageAspectFlags aspectflags);

        void createTextureSampler(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
        void createDescriptorSets(LogicalDevice logicaldevice, GraphicsPipeline pipeline, Pool pool, VertexBuffer vertexbuffer);
        void destroyDescriptorSet(LogicalDevice logicaldevice);
        void bindDescriptorSet(VertexBuffer vertexbuffer, GraphicsPipeline pipeline,uint32_t currentFrame);

        VkDescriptorSet& getdescriptorSets(int i){return descriptorSets[i];};
        VkImage& getTextureImage(){return textureImage;}
        VkDeviceMemory& getTextureImageMemory(){return textureImageMemory;}

    private:
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
        VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];

};

class DepthBuffer {
    public:
    void createDepthResources(PhysicalDevice physicaldevice, Texture texture, LogicalDevice logicaldevice, VertexBuffer buffer, Swapchain &swapchain);
    void destroyDepthResources(LogicalDevice logicaldevice);
    void cleanDepthResources(LogicalDevice logicaldevice);

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features,PhysicalDevice physicaldevice);
    VkFormat findDepthFormat(PhysicalDevice physicaldevice);
    bool hasStencilComponent(VkFormat format);
    VkImageView& getdepthImageView(){return depthImageView;}
    VkClearValue& getClearImageView(){return clearDepth;}
    
    VkExtent2D& getDepthSize(){return depthsize;}

    VkImage& getdepthImage(){return depthImage;}
    void recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline, uint32_t currentFrame, VertexBuffer& vertexbuffer, Texture texture, VkClearValue clearDepth, VkImage depthImage);


    private:
        VkExtent2D depthsize ;
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
    VkClearValue clearDepth;
};

