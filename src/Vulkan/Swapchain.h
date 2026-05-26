#pragma once

#include "Window.h"


class Swapchain { 
public:

	void CreateSwapChain(WindowSurface windowsurface, PhysicalDevice physicaldevice, Window window, LogicalDevice logicaldevice);
	void CreateImageView(LogicalDevice logicaldevice);
	void cleanSwapchain();
	void destroySwapchain(LogicalDevice logicaldevice);

	void RecreateSwapchain(LogicalDevice logicaldevices,WindowSurface windowsurface,PhysicalDevice physicaldevice,Window window);
	std::vector<VkImage>& getSwapchainImages() { return swapchainImages; } 
	std::vector<VkImageView>& getSwapchainImageView() { return swapChainImageViews; }
	VkSwapchainKHR getSwapchain() { return swapChain; }

	VkExtent2D getExtentSwapchain() { return swapChainExtent; }
	VkSurfaceFormatKHR GetswapChainSurfaceFormat() {return swapChainSurfaceFormat;}

private:

	VkSwapchainKHR swapChain;
	VkExtent2D swapChainExtent;
	VkSurfaceFormatKHR swapChainSurfaceFormat;
	VkFormat swapChainImageFormat;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapChainImageViews;


};
