#pragma once
#include "../Buffers/VulkanVertexBuffer.h"
#include "../Helpers/VulkanConstants.h"
#include "../Core/VulkanContext.h"
#include "../../../Assets/Importer/CPUModel.h"
#include "../../../Renderer/Camera.h"
#include <iostream>
#include <chrono>


// Agrupacion de vertices temporalmente aqui para la creacion del indexbuffe
class VulkanTexture : public VertexBuffer {
    public:
    VulkanTexture();
          ~VulkanTexture();


        void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkPhysicalDevice physicalDevice);
    void createTextureImage(
    VkDevice device,
    VkPhysicalDevice physicalDevice,
    VkCommandPool commandPool,
    VkCommandBuffer& commandBuffer,
    VkQueue graphicsQueue,
    const CpuModel& model);    
     
        
        void transitionImageLayout(VkImage &image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

void createImage(uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
     VkImage& image, VkDeviceMemory& imageMemory , uint32_t mipLevels, VkPhysicalDevice physicaldevice);
        
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void destroyBuffer() override;

       void createTextureImageView();

  void createImageViews(VkImageAspectFlags aspectflags, std::vector<VkImageView> swapChainImageViews,std::vector<VkImage> swapchainImages,VkFormat swapChainImageFormat);
VkImageView createImageView(VkImage image, VkFormat format,  VkImageAspectFlags aspectFlags, uint32_t mipLevels);
 void createTextureSampler(VkPhysicalDevice physicalDevice);

int GetTextureWidth(int i) {return texWidth[i];};
int GetTextureHeight(int i) {return texHeight[i];};



        const std::vector<VkImageView>&
        GetTextureImageViewRef() const
        {
            return textureImageView;
        } 
           //std::vector<VkSampler> GetTextureSamplerRef() {return textureSampler;};

    VkImageView GetTextureImageView(int i) {return textureImageView[i];};
    VkSampler GetTextureSampler() {return textureSampler;};

    void destroyImageTextureView();







    private:
    std::vector<uint32_t> mipLevels;
    std::vector<VkImage> textureImages;
    std::vector<VkDeviceMemory> textureImageMemories;

    std::vector<VkImageView> textureImageView;
    VkSampler textureSampler= VK_NULL_HANDLE;

    std::vector<int> texWidth, texHeight, texChannels;

    VkCommandBuffer cp_commandBuffer;
  
    VkQueue cp_graphicsQueue;
    std::vector<CpuMaterial> cp_materials;
};