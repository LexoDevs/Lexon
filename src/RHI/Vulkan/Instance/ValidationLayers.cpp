#include "ValidationLayers.h"

void VulkanValidation::DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Context.instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(m_Context.instance, m_Context.debuginfo, pAllocator);
    }
}

void VulkanValidation::DestroyValidationLayers() {
	if (enableValidationLayers) {
           DestroyDebugUtilsMessengerEXT(nullptr);
        }

};


VkResult VulkanValidation::CreateDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_Context.instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
    const VkDebugUtilsMessengerCreateInfoEXT info = m_Context.createinfodebug;

        return func(m_Context.instance, &info, pAllocator, &m_Context.debuginfo);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanValidation::populateDebugMessengerCreateInfo() {
	m_Context.createinfodebug = {};
	m_Context.createinfodebug.sType 				= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	m_Context.createinfodebug.messageSeverity 		= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	m_Context.createinfodebug.messageType 			= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	m_Context.createinfodebug.pfnUserCallback 		= debugCallback;
}

void VulkanValidation::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	populateDebugMessengerCreateInfo();

	if (CreateDebugUtilsMessengerEXT(nullptr) != VK_SUCCESS) {
	throw std::runtime_error("failed to set up debug messenger!");
	}
}

bool checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}