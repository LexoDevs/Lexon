#pragma once

#include "../Helpers/VulkanConstants.h"
#include "../Core/VulkanContext.h"
#include "vector"

#include <iostream>

struct MessageVisibility{
    bool error=true;
    bool warning=false;
    bool severity=true; 
    bool verbose=false;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    MessageVisibility selected;

    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT && selected.error)
        std::cerr << "\033[1;31m[VALIDATION ERROR] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT && selected.warning)
        std::cout << "\033[1;33m[VALIDATION WARNING] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT && selected.severity)
        std::cout << "\033[1;36m[VALIDATION INFO] \033[0m" << pCallbackData->pMessage << std::endl;
    
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT && selected.verbose)
        std::cout << "\033[1;36m[VERBOSE INFO] \033[0m" << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
class VulkanValidation {
public:

    VulkanValidation();
    ~VulkanValidation();

    bool Init(VkInstance instance);
    void setupDebugMessenger();
    void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
    VkResult CreateDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);
    void populateDebugMessengerCreateInfo();
    void DestroyValidationLayers();

private:
	//Contexto de vulkan (Datos)

	bool m_Initialized = false;
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT debuginfo;
    VkDebugUtilsMessengerCreateInfoEXT createinfodebug;
};
