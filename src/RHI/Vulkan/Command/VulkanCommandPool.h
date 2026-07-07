#pragma once

#include "../Core/VulkanContext.h"
#include <iostream>

class VulkanCommandPool {
public:
    VulkanCommandPool(VulkanContext& context)
        : m_Context(context)
    {
    }

    void createCommandPool();
    void destroyCommandPool();

    private:
        VulkanContext& m_Context;
};