#pragma once

#include "../Core/VulkanContext.h"

class VulkanSwapchain { 
public:
    VulkanSwapchain();
    ~VulkanSwapchain();

    
	void CreateSwapChain(VkDevice device, VkSurfaceKHR surface, SwapchainSupportDetails details,QueueFamilyIndices QueueFamilie);
	void DestroySwapchain();
	void CreateImageView();

	VkSwapchainKHR GetSwapchain(){return swapchain;};
	VkExtent2D GetSwapchainExtent() {return swapChainExtent;};

	std::vector<VkImage> GetSwapchainImagesRef(){return swapchainImages;};
	VkImage GetSwapchainImages(uint32_t frame ){return swapchainImages[frame];};
	VkFormat GetSwapChainImageFormat() {return swapChainImageFormat;};
	VkSurfaceFormatKHR GetSwapChainSurfaceFormat() {return swapChainSurfaceFormat;};

	VkImageView GetSwapChainImageViews(uint32_t frame ){return swapChainImageViews[frame];};

	//void cleanSwapchain();
	//void destroySwapchain();

	//void RecreateSwapchain();
	VkDevice cp_device = VK_NULL_HANDLE;


private:
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	VkExtent2D swapChainExtent;
	VkSurfaceFormatKHR swapChainSurfaceFormat;
	VkFormat swapChainImageFormat;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapChainImageViews;






};