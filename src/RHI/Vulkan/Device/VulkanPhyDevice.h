#pragma once
#include "../Core/VulkanContext.h"
#include "../Extensions/VulkanExtensions.h"

#include <string>
#include <iostream>
#include <set>

#include <VulkanFormats.h>

class VulkanPhysicalDevice {
public:
    VulkanPhysicalDevice();
	void SelectPhysicalDevices(VkInstance instance);
    void DestroyPhysicalDevices();
    void CreateSwapchainSupportDetails(VkSurfaceKHR Surface);
    VkPhysicalDevice GetPhysicalDevice(){return physicalDevice;};
    SwapchainSupportDetails GetSwapDetails(){return swapDetails;};

private:
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(VkSurfaceKHR Surface);
		VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkSurfaceKHR Surface);
		VkPresentModeKHR GetSurfacePresentationsMode(VkSurfaceKHR Surface);
		std::vector<VkSurfaceFormatKHR> getSurfaceFormats(VkSurfaceKHR Surface, uint32_t &pSurfaceFormatCount);
		VkExtent2D chooseSwapExtent();

        
        VkPhysicalDevice physicalDevice;
        SwapchainSupportDetails swapDetails;

    bool IsDeviceSelectable(VkPhysicalDevice physicaldevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice physicaldevice);

	
};