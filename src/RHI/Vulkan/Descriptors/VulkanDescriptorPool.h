#pragma once

#include "../Core/VulkanContext.h"
#include <iostream>
#include "../Helpers/VulkanConstants.h"
#include <vulkan/vulkan.h>
#include <array>

class VulkanDescriptorPool {
public:
    VulkanDescriptorPool(){};
    ~VulkanDescriptorPool();

    void createDescriptorPool(VkDevice device);
    void destroyDescriptorPool();

    void CreateImGuiDescriptorPool(VkDevice device);
    VkDescriptorPool GetDescriptorPool(){ return descriptorPool;};

private:
    VkDescriptorPool descriptorPool;
    VkDevice cp_device;

    bool initialiced = false;

};