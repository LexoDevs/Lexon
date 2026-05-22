#pragma once

#include "Buffers.h"

const int MAX_FRAMES_IN_FLIGHT = 2;


class Render {

    public:
    void createSyncObjects(LogicalDevice device);
    void destroyFences(LogicalDevice logicaldevice);
    void drawFrame(LogicalDevice logicaldevice, Pool pool, Swapchain swapchain, GraphicsPipeline pipeline);

    private:
    VkSemaphore imageAvailableSemaphore;   // "presentComplete"
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
    uint32_t frameIndex = 0;

};