#pragma once

#include "../Core/VulkanContext.h"

class VulkanSwapchain { 
public:
    VulkanSwapchain()

    {
    }
    
	void CreateSwapChain(VkDevice device, VkSurfaceKHR surface, SwapchainSupportDetails details);
	void CreateImageView();
	void cleanSwapchain();
	void destroySwapchain();

	//void RecreateSwapchain();
    VkSwapchainKHR swapchain;
	VkExtent2D swapChainExtent;
	VkSurfaceFormatKHR swapChainSurfaceFormat;
	VkFormat swapChainImageFormat;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapChainImageViews;
private:







};