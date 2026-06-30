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
#include "../RHI/Vulkan/VulkanRHI.h"

#include <iostream>
#include <optional>
#include <set>
#include <string>
#include <bitset>



struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};




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