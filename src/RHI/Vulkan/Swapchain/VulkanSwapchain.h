#pragma once

#include "../Core/VulkanContext.h"

class VulkanSwapchain { 
public:
    VulkanSwapchain(VulkanContext& context)
        : m_Context(context)
    {
    }
    
	void CreateSwapChain();
	void CreateImageView();
	void cleanSwapchain();
	void destroySwapchain();

	void RecreateSwapchain();



private:
	//Contexto de vulkan (Datos)
    VulkanContext& m_Context;





};