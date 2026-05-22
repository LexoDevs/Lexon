#pragma once

#include "Pipeline.h"

class Pool {
public:
    void createCommandPool(LogicalDevice logicaldevice, PhysicalDevice physicaldevice);
    void destroyCommandPool(LogicalDevice logicaldevice);
    void createCommandBuffer(LogicalDevice logicaldevice);
    void recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline);
    VkCommandBuffer getCommandBuffer(){ return commandBuffer;};
    

private:
    VkCommandPool commandPool = nullptr;
    VkCommandBuffer commandBuffer;


};

