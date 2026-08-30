#include "ValidationLayers.h"

VulkanValidation::VulkanValidation()
    {
        if (m_Initialized)
    {
        return;
    }
        m_Initialized = true;
    };

VulkanValidation::~VulkanValidation(){
    if (!m_Initialized)
    {
        return;
    }

	if (m_instance != VK_NULL_HANDLE)
    {

    }
    m_Initialized = false;
};

bool VulkanValidation::Init(VkInstance instance){
	m_instance = instance;
    setupDebugMessenger();
	
	return true;
};

void VulkanValidation::DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {

    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(m_instance, debuginfo, pAllocator);
    }
}

void VulkanValidation::DestroyValidationLayers() {

	if (enableValidationLayers) {
           DestroyDebugUtilsMessengerEXT(nullptr);
        }
	std::cout<<"Se han destruido las capas de validacion"<<std::endl;

};


VkResult VulkanValidation::CreateDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
    const VkDebugUtilsMessengerCreateInfoEXT info = createinfodebug;

        return func(m_instance, &info, pAllocator, &debuginfo);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanValidation::populateDebugMessengerCreateInfo() {
	createinfodebug = {};
	createinfodebug.sType 				= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createinfodebug.messageSeverity 		=  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createinfodebug.messageType 			= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createinfodebug.pfnUserCallback 		= debugCallback;
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