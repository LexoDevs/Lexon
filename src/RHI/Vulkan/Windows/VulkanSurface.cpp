
#include "VulkanSurface.h"

#include <cstdint> //uint32_t
#include <limits> //std::numeric_limits
#include <algorithm> //std::clamp
//############## Clase Surface Window ################//

void VulkanSurface::CreateWindowSurface() {

	std::cout << "\033[1;36m[!] Creando superficie de ventana virtual...\033[0m\n";


    if (glfwCreateWindowSurface(m_Context.instance, m_Context.GLFWwindow, nullptr, &m_Context.surface) != 0) {
        throw std::runtime_error("failed to create window surface!");
    }

    std::cout << "\t\033[1;32mWindow Surface\033[0m:" << " alojada en \033[1;32m" << &m_Context.surface << "\033[0m\n\n";

};




VkSurfaceCapabilitiesKHR VulkanSurface::GetSurfaceCapabilities() {

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Context.physicalDevice, m_Context.surface, &m_Context.surfaceCapabilities);

    std::cout << "\t\033[1;33mLa resolucion de la ventana es: \033[0m\033[1;32m" << m_Context.surfaceCapabilities.currentExtent.height << 'x' << m_Context.surfaceCapabilities.currentExtent.width << "\033[0m\n\n";

    return m_Context.surfaceCapabilities;
}

VkExtent2D VulkanSurface::chooseSwapExtent() {

    if (m_Context.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return m_Context.surfaceCapabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(m_Context.GLFWwindow, &width, &height);



    
    width = std::clamp<uint32_t>(width, m_Context.surfaceCapabilities.minImageExtent.width, m_Context.surfaceCapabilities.maxImageExtent.width);
    height = std::clamp<uint32_t>(height, m_Context.surfaceCapabilities.minImageExtent.height, m_Context.surfaceCapabilities.maxImageExtent.height);
    
    m_Context.surfaceCapabilities.currentExtent.width = width;
    m_Context.surfaceCapabilities.currentExtent.height = height;

    return { width , height};
};

std::vector<VkSurfaceFormatKHR> VulkanSurface::getSurfaceFormats(uint32_t &pSurfaceFormatCount) {
    pSurfaceFormatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_Context.physicalDevice, m_Context.surface, &pSurfaceFormatCount, NULL);
    std::vector<VkSurfaceFormatKHR> availableFormats(pSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(m_Context.physicalDevice, m_Context.surface, &pSurfaceFormatCount, availableFormats.data());
    m_Context.availableFormats = availableFormats ;

    return m_Context.availableFormats;
}

VkSurfaceFormatKHR VulkanSurface::chooseSwapSurfaceFormat() {
     uint32_t pSurfaceFormatCount = 0;

    getSurfaceFormats(pSurfaceFormatCount);
    std::cout << "\t\033[1;33mSe detectaron un total de \033[0m\033[1;32m" << pSurfaceFormatCount << "\033[0m\033[1;33m formatos\033[0m\n";
    for (int i = 0; i < m_Context.availableFormats.size()-1; i++) {

        if (FormatToString(m_Context.availableFormats[i].format) == "VK_FORMAT_B8G8R8A8_UNORM") {

            std::cout << "\t\t\033[1;33mEl formato seleccionado es: \033[0m\033[1;32m" << FormatToString(m_Context.availableFormats[i].format) << "\033[0m\n\n";
            m_Context.surfaceformats = m_Context.availableFormats[i];
            
            return m_Context.availableFormats[i];
        }
    }
    return {};
}

VkPresentModeKHR VulkanSurface::GetSurfacePresentationsMode() {
    uint32_t pPresentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_Context.physicalDevice, m_Context.surface, &pPresentModeCount, NULL);
    std::vector<VkPresentModeKHR> availablePresentModes(pPresentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_Context.physicalDevice, m_Context.surface, &pPresentModeCount, availablePresentModes.data());
    std::cout << "\t\033[1;33mSe detectaron un total de \033[0m\033[1;32m" << pPresentModeCount << "\033[0m\033[1;33m modos de presentacion\033[0m\n";


    for (int i = 0; i < availablePresentModes.size(); i++) {

        if (PresentModeToString(availablePresentModes[i]) == "VK_PRESENT_MODE_MAILBOX_KHR") {

            std::cout << "\t\t\033[1;33mEl formato seleccionado es: \033[0m\033[1;32m" << PresentModeToString(availablePresentModes[i]) << "\033[0m\n\n";
            return availablePresentModes[i];

        }
    }
    return {};
}

void VulkanSurface::DestroyVulkanSurface(){
    vkDestroySurfaceKHR(m_Context.instance, m_Context.surface, nullptr);
	};