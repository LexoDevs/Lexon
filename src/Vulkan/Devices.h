#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <vulkan/vulkan.h> 
#include "../Runtime/RHI/Backends/Vulkan/VulkanRHI.h"

#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <bitset>

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

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};


/*class VulkanInstance {
public:

	void CreateInstance();
	void DestroyInstance();
	VkInstance& GetInstance() { return instance; };

private:
	VkInstance instance;

	void GetInstanceVersion();
	std::vector<const char*> GetInstanceExtensionsRequired();

};*/

class PhysicalDevice {
public:
	void SelectPhysicalDevices(VulkanInstance instance);
	VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; };

private:
	bool IsDeviceSelectable(VkPhysicalDevice physicalDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

	VkPhysicalDevice physicalDevice = nullptr;
	
};

class LogicalDevice {
public:
	void CreateLogicalDevice(PhysicalDevice physicaldevice);
	VkDevice& GetLogicalDevice() { return logicaldevice; };
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	QueueFamilyIndices& GetQueueFamilies() { return QueueFamilie; };
	VkQueue getGraphicQueue() { return graphicsQueue; };
	VkQueue getPresentQueue() { return presentQueue; };

	void SetQueueFamilies(QueueFamilyIndices newQueueFamilie) {
		QueueFamilyIndices QueueFamilie = newQueueFamilie;
	}

private:


	QueueFamilyIndices QueueFamilie;
	VkDevice logicaldevice;
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
};