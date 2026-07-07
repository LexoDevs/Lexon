#pragma once
#include "../Core/VulkanContext.h"
#include "../Helpers/VulkanConstants.h"
#include <iostream>
#include "../../../Assets/Loaders/LoaderAssets.h"
#include <chrono>
#include "../../../Renderer/Camera.h"

class BufferI {
    public:
    BufferI(VulkanContext& context)
        : m_Context(context)
    {
    }

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands();
        void createImage(uint32_t width,
             uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

        private:
            VulkanContext& m_Context;

};

//------------------------------HIJOS--------------------------//

    class VertexBuffer : public BufferI {
        public:
        VertexBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createVertexBuffer(ObjectInstance& mesh);
            void destroyVertexBuffer();
            void setVertexBuffer(VkBuffer new_vertexbuffer){  m_Context.vertexBuffer = new_vertexbuffer;}

        private:
            VulkanContext& m_Context;



    };



    class IndexBuffer : public BufferI {
        public:
        IndexBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createIndexBuffer(ObjectInstance& mesh);
            void destroyIndexBuffer();

        private:
            VulkanContext& m_Context;


    };






    class UniformBuffer : public BufferI {
        public:
        UniformBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createUniformBuffer();
            void destroyUniformBuffer();
            void updateUniformBuffer(uint32_t currentImage, ObjectInstance mesh, CameraView camera);

        private:
            VulkanContext& m_Context;



    };

    class DepthBuffer : public BufferI {
        


        public:
        DepthBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              

    {
    }
        void createDepthResources();
        void destroyDepthResources();
        void cleanDepthResources();

        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);

        void recordCommandBuffer(uint32_t imageIndex, uint32_t currentFrame, ObjectInstance mesh);


        private:
            VulkanContext& m_Context;



    };