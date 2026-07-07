#pragma once
#include <vector>
#include <vulkan/vulkan.h>

static std::vector<const char*> requiredDeviceExtension = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	
};

static const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
	
};