#include "VulkanDepthBuffer.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


DepthBuffer::~DepthBuffer(){
destroyBuffer();
};

void DepthBuffer::createDepthFormat(VkPhysicalDevice physicalDevice){

        depthformat = findDepthFormat(physicalDevice);


}

void DepthBuffer::createDepthResources(VkDevice device,VkExtent2D swapChainExtent,VkPhysicalDevice physicalDevice){
    cp_device = device;
        createDepthFormat(physicalDevice);
        
        depthsize.width  = swapChainExtent.width;
    depthsize.height = swapChainExtent.height;

        createImage(depthsize.width, depthsize.height, depthformat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory, 1,physicalDevice);
        depthImageView = createImageView(depthImage, depthformat, VK_IMAGE_ASPECT_DEPTH_BIT, 1 );
        
};


VkFormat DepthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                depthformat = format;
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                depthformat = format;
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
    }


VkFormat DepthBuffer::findDepthFormat(VkPhysicalDevice physicalDevice) {
    return findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,physicalDevice);
}


bool DepthBuffer::hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

void DepthBuffer::destroyBuffer(){
    std::cout<<"depth Buffer destruido"<<std::endl;

    vkDestroyImageView(cp_device, depthImageView, nullptr);
        std::cout<<"depth Buffer destruido"<<std::endl;

    vkDestroyImage(cp_device, depthImage, nullptr);
        std::cout<<"depth Buffer destruido"<<std::endl;

    vkFreeMemory(cp_device, depthImageMemory, nullptr);
        std::cout<<"depth Buffer destruido"<<std::endl;

};

void DepthBuffer::cleanDepthResources(){

    vkDestroyImage(cp_device, depthImage, nullptr);

};











void DepthBuffer::createImage(uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory , uint32_t mipLevels, VkPhysicalDevice physicaldevice) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(cp_device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(cp_device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties,physicaldevice);

        if (vkAllocateMemory(cp_device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(cp_device, image, imageMemory, 0);
}


VkImageView DepthBuffer::createImageView(VkImage image, VkFormat format,  VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(cp_device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view!");
        }

        return imageView;
    }