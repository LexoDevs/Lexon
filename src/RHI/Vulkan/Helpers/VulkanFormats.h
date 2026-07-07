#pragma once

#include <vulkan/vulkan.h> 

#include <optional>

static const char* FormatToString(VkFormat format) {
    switch (format) {
    case 44:              return "VK_FORMAT_B8G8R8A8_UNORM";
    case 50:              return "VK_FORMAT_B8G8R8A8_SRGB ";
    case 37:                 return "VK_FORMAT_R8G8B8A8_UNORM ";
    case 43:               return "VK_FORMAT_R8G8B8A8_SRGB ";
    case 64:               return "VK_FORMAT_A2B10G10R10_UNORM_PACK32 ";

    default: return "tochar(format)";
    }
}

static const char* PresentModeToString(VkPresentModeKHR presentmode) {
    switch (presentmode) {
    case 0:                  return "VK_PRESENT_MODE_IMMEDIATE_KHR";
    case 1:                  return "VK_PRESENT_MODE_MAILBOX_KHR";
    case 2:                  return "VK_PRESENT_MODE_FIFO_KHR";
    case 3:                  return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
    case 1000111000:         return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
    case 1000111001:         return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
    case 1000361000:         return "VK_PRESENT_MODE_FIFO_LATEST_READY_KHR";

    default: return "Formato desconocido.";
    }
}

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() const {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};
