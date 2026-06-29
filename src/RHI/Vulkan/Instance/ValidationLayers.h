#pragma once

#include "../Core/VulkanContext.h"
#include "vector"

class ValidationLayers {
public:

	VkDebugUtilsMessengerEXT GetDebugInfos() { return m_context.debuginfo; };

private:
    VulkanContext& m_context;


};