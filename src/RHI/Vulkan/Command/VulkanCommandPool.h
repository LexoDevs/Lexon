#pragma once

#include "../Core/VulkanContext.h"
#include <iostream>

class VulkanCommandPool {
    public:
        VulkanCommandPool();
        ~VulkanCommandPool();

        void createCommandPool(VkDevice device, QueueFamilyIndices QueueFamilie);
        void destroyCommandPool();
        VkCommandPool GetHandle() {return commandPool;};


    private:
        VkCommandPool commandPool;
        VkDevice cp_device;
};