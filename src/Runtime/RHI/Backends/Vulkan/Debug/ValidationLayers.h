#pragma once

#include "../Core/VulkanContext.h"
#include "vector"

class VulkanInstance {
public:

	VkDebugUtilsMessengerEXT GetDebugInfos() { return context.debuginfo; };

private:
    VulkanContext& context;


};