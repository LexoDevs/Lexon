#pragma once
#include "VulkanBuffer.h"
#include "../Core/VulkanContext.h"


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

        void createDepthFormat();

        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);

        void recordCommandBuffer(uint32_t imageIndex, uint32_t currentFrame, ObjectInstance mesh);


        private:
            VulkanContext& m_Context;



    };