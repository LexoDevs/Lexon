#pragma once

#include "../Core/VulkanContext.h"
#include <iostream>
#include "../Helpers/VulkanFormats.h"


class VulkanSurface {

	public:
        VulkanSurface();
        ~VulkanSurface();

		void CreateWindowSurface(void* window, VkInstance instance);
		void DestroyVulkanSurface();

		VkSurfaceKHR GetSurface(){return Surface;};



	private:
		VkInstance cp_Instance = VK_NULL_HANDLE;
    	VkSurfaceKHR Surface = VK_NULL_HANDLE;
    	GLFWwindow* cp_Window = nullptr;



};
