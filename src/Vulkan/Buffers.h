#pragma once

#include "VulkanConstants.h"

#include "Pipeline.h"
class Pool {
public:
    void createCommandPool(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
    void destroyCommandPool(LogicalDevice logicaldevice);
    void createCommandBuffer(LogicalDevice logicaldevice);
    void recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline, uint32_t currentFrame);
    VkCommandBuffer& getCommandBuffer(int i){ return commandBuffers[i];};
    

private:
    VkCommandPool commandPool = nullptr;
    VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT];


};

