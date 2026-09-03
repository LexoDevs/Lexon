#pragma once
#include "VulkanVertexBuffer.h"
#include "../Core/VulkanContext.h"


class DepthBuffer : public VertexBuffer {
        


        public:
        DepthBuffer(){};
        ~DepthBuffer();


        void createDepthResources(VkDevice device,VkExtent2D swapChainExtent,VkPhysicalDevice physicalDevice);
        void destroyBuffer() override;
        void cleanDepthResources();

        void createDepthFormat(VkPhysicalDevice physicalDevice);

        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
        VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
        bool hasStencilComponent(VkFormat format);

void createImage(uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
             VkImage& image, VkDeviceMemory& imageMemory , uint32_t mipLevels, VkPhysicalDevice physicaldevice);

             VkImageView createImageView(VkImage image, VkFormat format,  VkImageAspectFlags aspectFlags, uint32_t mipLevels);
            VkImage GetDepthImage(){return depthImage;}
            VkImageView GetDepthImageView() {return depthImageView;};

        private:
            VkExtent2D depthsize;
            VkImage depthImage;
            VkDeviceMemory depthImageMemory;
            VkImageView depthImageView;
            VkClearValue clearDepth;
            VkFormat depthformat;


//VkDevice cp_device;
    };