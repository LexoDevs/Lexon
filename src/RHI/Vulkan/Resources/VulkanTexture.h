#pragma once
#include "../Buffers/VulkanVertexBuffer.h"
#include "../Helpers/VulkanConstants.h"
#include "../Core/VulkanContext.h"

#include "../../../Renderer/Camera.h"
#include <iostream>
#include <chrono>


// Agrupacion de vertices temporalmente aqui para la creacion del indexbuffe
class VulkanTexture : public BufferI {
    public:
    VulkanTexture(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              

    {
    }
        void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
        void createTextureImage();
        
        void transitionImageLayout(VkImage &image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);


        
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void destroyImageTexture();

        void createTextureImageView();
        void destroyImageTextureView();

        void createImageViews(VkImageAspectFlags aspectflags);

        void createTextureSampler();

    private:
                VulkanContext& m_Context;

};