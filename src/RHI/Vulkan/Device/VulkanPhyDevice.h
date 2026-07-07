#pragma once
#include "../Core/VulkanContext.h"
#include "../Extensions/VulkanExtensions.h"

#include <string>
#include <iostream>
#include <set>

struct VulkanContext;

class VulkanPhysicalDevice {
public:
    VulkanPhysicalDevice(VulkanContext& context)
        : m_Context(context)
    {
    }

	void SelectPhysicalDevices();
    void DestroyPhysicalDevices();
private:
    VulkanContext& m_Context;


    bool IsDeviceSelectable(VkPhysicalDevice physicaldevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice physicaldevice);

	
};