#pragma once

#include "../Resources/VulkanConstants.h"
#include "../Core/VulkanContext.h"
#include "vector"

#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        std::cerr << "\033[1;31m[VALIDATION ERROR] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        std::cout << "\033[1;33m[VALIDATION WARNING] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
        std::cout << "\033[1;36m[VALIDATION INFO] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else
        std::cout << "\033[1;90m[VALIDATION VERBOSE] \033[0m" << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
class VulkanValidation {
public:

    VulkanValidation(VulkanContext& context)
        : m_Context(context)
    {
    }

    void DestroyDebugUtilsMessengerEXT(VulkanContext context, const VkAllocationCallbacks* pAllocator);
    VkResult CreateDebugUtilsMessengerEXT(VulkanContext context, const VkAllocationCallbacks* pAllocator);

	VkDebugUtilsMessengerEXT GetDebugInfos() { return m_Context.debuginfo; };


private:
	//Contexto de vulkan (Datos)
    VulkanContext& m_Context;

    VkDebugUtilsMessengerCreateInfoEXT createinfodebug;
};
