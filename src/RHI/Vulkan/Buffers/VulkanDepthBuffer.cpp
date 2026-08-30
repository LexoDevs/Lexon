#include "VulkanDepthBuffer.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


void DepthBuffer::createDepthFormat(){

        m_Context.depthformat = findDepthFormat();


}

void DepthBuffer::createDepthResources(){
        createDepthFormat();
        
        m_Context.depthsize.width  = m_Context.swapChainExtent.width;
    m_Context.depthsize.height = m_Context.swapChainExtent.height;

        createImage(m_Context.depthsize.width, m_Context.depthsize.height, m_Context.depthformat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context.depthImage, m_Context.depthImageMemory, 1);
        m_Context.depthImageView = createImageView(m_Context.depthImage, m_Context.depthformat, VK_IMAGE_ASPECT_DEPTH_BIT, 1 );
        
};


VkFormat DepthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_Context.physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                m_Context.depthformat = format;
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                m_Context.depthformat = format;
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
    }


VkFormat DepthBuffer::findDepthFormat() {
    return findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


bool DepthBuffer::hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

void DepthBuffer::destroyDepthResources(){

    vkDestroyImageView(m_Context.device, m_Context.depthImageView, nullptr);
    vkDestroyImage(m_Context.device, m_Context.depthImage, nullptr);
    vkFreeMemory(m_Context.device, m_Context.depthImageMemory, nullptr);
};

void DepthBuffer::cleanDepthResources(){

    vkDestroyImage(m_Context.device, m_Context.depthImage, nullptr);

};














