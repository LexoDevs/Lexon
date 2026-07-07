#pragma once
#include "../Extensions/VulkanExtensions.h"
#include "../Helpers/VulkanFormats.h"
#include <optional>

#include "../Core/VulkanContext.h"


class VulkanLogicalDevice {
public:
    VulkanLogicalDevice(VulkanContext& context)
        : m_Context(context)
    {
    }


	void CreateLogicalDevice();
	QueueFamilyIndices findQueueFamilies();
    void DestroyLogicalDevice();



private:
    VulkanContext& m_Context;


};