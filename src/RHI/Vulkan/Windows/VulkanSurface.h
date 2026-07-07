#pragma once

#include "../Core/VulkanContext.h"
#include <iostream>
#include "../Helpers/VulkanFormats.h"


class VulkanSurface {

	public:
        VulkanSurface(VulkanContext& context)
            : m_Context(context)
        {
        }

		void CreateWindowSurface();
		VkSurfaceCapabilitiesKHR GetSurfaceCapabilities();
		VkPresentModeKHR GetSurfacePresentationsMode();
		VkExtent2D chooseSwapExtent();
		std::vector<VkSurfaceFormatKHR> getSurfaceFormats(uint32_t &pSurfaceFormatCount);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat();
		void DestroyVulkanSurface();


	private:

		VulkanContext& m_Context;
};
