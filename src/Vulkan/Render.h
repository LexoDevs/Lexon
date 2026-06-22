#pragma once

#include "Buffers.h"

#include "VulkanConstants.h"

class Render {

    public:
    void createSyncObjects(LogicalDevice device);
    void destroyFences(LogicalDevice logicaldevice);
    void cleanSync(LogicalDevice logicaldevice);
    void drawFrame(LogicalDevice logicaldevice, Pool pool, Swapchain& swapchain, GraphicsPipeline pipeline,
                        WindowSurface windowsurface, PhysicalDevice physicaldevice, Window window,VertexBuffer vertexBuffer,
                        Texture texture, DepthBuffer &depthbuffer, VkClearValue clearDepth, VkImage depthImage);

    private:
    VkSemaphore imageAvailableSemaphore[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore renderFinishedSemaphore[MAX_SWAPCHAIN_IMAGES]; // Uno por imagen del swapchain
    VkFence inFlightFence[MAX_FRAMES_IN_FLIGHT];
    uint32_t frameIndex = 0;




};


