#pragma once

#include "../Core/VulkanContext.h"
#include <iostream>
#include "../Helpers/VulkanConstants.h"
#include <vulkan/vulkan.h>
#include <array>

class VulkanDescriptorPool {
public:
    VulkanDescriptorPool(VulkanContext& context)
        : m_Context(context)
    {
    }
    void createDescriptorPool();
    void destroyDescriptorPool();

    VkDescriptorPool CreateImGuiDescriptorPool();

private:
        VulkanContext& m_Context;


};