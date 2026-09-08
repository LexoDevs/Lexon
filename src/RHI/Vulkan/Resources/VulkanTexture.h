#pragma once
#include "../Buffers/VulkanVertexBuffer.h"
#include "../Helpers/VulkanConstants.h"
#include "../Core/VulkanContext.h"

#include "../../../Renderer/Camera.h"
#include <iostream>
#include <chrono>


// Agrupacion de vertices temporalmente aqui para la creacion del indexbuffe
class VulkanTexture : public VertexBuffer {
    public:
    VulkanTexture();
          ~VulkanTexture();


        void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkDevice device, VkCommandBuffer commandBuffer,VkQueue graphicsQueue);
        void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool,
     VkCommandBuffer commandBuffer, VkQueue graphicsQueue);
        
        void transitionImageLayout(VkImage &image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

void createImage(uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
     VkImage& image, VkDeviceMemory& imageMemory , uint32_t mipLevels, VkPhysicalDevice physicaldevice);
        
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void destroyBuffer();

//        void createTextureImageView();

    //    void createImageViews(VkImageAspectFlags aspectflags);

      //  void createTextureSampler();

    private:
    std::vector<uint32_t> mipLevels;
    std::vector<VkImage> textureImages;
    std::vector<VkDeviceMemory> textureImageMemories;

    std::vector<int> texWidth, texHeight, texChannels;

    VkCommandBuffer cp_commandBuffer;
    VkQueue cp_graphicsQueue;
};