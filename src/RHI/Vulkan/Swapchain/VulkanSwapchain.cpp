#include "VulkanSwapchain.h"
#include <iostream>
#include "../Helpers/VulkanFormats.h"

void VulkanSwapchain::CreateSwapChain() 
{
    std::cout << "\033[1;36m[!] Creando cadena de intercambio...\033[0m\n";

    m_Context.swapChainExtent = m_Context.surfaceCapabilities.currentExtent;

    // Formato
    std::vector<VkSurfaceFormatKHR> availableFormats = m_Context.availableFormats;

    m_Context.swapChainSurfaceFormat = m_Context.surfaceformats;

    // Present mode
    VkPresentModeKHR presentationmode = m_Context.presentationsurface;

    // Número de imágenes
    uint32_t imageCount = m_Context.surfaceCapabilities.minImageCount + 1;

    if (m_Context.surfaceCapabilities.maxImageCount > 0 && imageCount > m_Context.surfaceCapabilities.maxImageCount) {
        imageCount = m_Context.surfaceCapabilities.maxImageCount;
    }


    // Crear Swapchain
    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType              = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface            = m_Context.surface;
    swapChainCreateInfo.minImageCount      = imageCount;
    swapChainCreateInfo.imageFormat        = m_Context.swapChainSurfaceFormat.format;
    swapChainCreateInfo.imageColorSpace    = m_Context.swapChainSurfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent        = m_Context.swapChainExtent;
    swapChainCreateInfo.imageArrayLayers   = 1;
    swapChainCreateInfo.imageUsage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.preTransform       = m_Context.surfaceCapabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha     = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode        = presentationmode;
    swapChainCreateInfo.clipped            = VK_TRUE;


    QueueFamilyIndices indices = m_Context.QueueFamilie;

    std::cout << "\033[1;36m[!] 2\033[0m"<<indices.graphicsFamily.value()<<"\n";

    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(),
                                      indices.presentFamily.value() };
    
    std::cout << "\033[1;36m[!] 1\033[0m\n";

    if (m_Context.QueueFamilie.graphicsFamily != m_Context.QueueFamilie.presentFamily) {
        swapChainCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
    } else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    // Crear el swapchain
    if (vkCreateSwapchainKHR(m_Context.device, &swapChainCreateInfo, nullptr, &m_Context.swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::cout << "\t\033[1;32mSwapchain\033[0m creada correctamente en \033[1;32m" << m_Context.swapchain << "\033[0m\n";

    // Obtener las imágenes
    vkGetSwapchainImagesKHR(m_Context.device, m_Context.swapchain, &imageCount, nullptr);
    m_Context.swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_Context.device, m_Context.swapchain, &imageCount, m_Context.swapchainImages.data());

    m_Context.swapChainImageFormat = m_Context.swapChainSurfaceFormat.format;

    std::cout << "\t\033[1;33mSwapchain creado con \033[0m\033[1;32m" << imageCount << "\033[0m\033[1;33m imagenes\033[0m\n";

    for (int i = 0; i<imageCount; i++){
        std::cout << m_Context.swapchainImages[i]<<"\n";
    }
}



void VulkanSwapchain::CreateImageView()
{
    m_Context.swapChainImageViews.resize(m_Context.swapchainImages.size());



    for (size_t i = 0; i < m_Context.swapchainImages.size(); i++) {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = m_Context.swapChainSurfaceFormat.format;
        imageViewCreateInfo.image = m_Context.swapchainImages[i];
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_Context.device, &imageViewCreateInfo, nullptr, &m_Context.swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
};

void VulkanSwapchain::cleanSwapchain()
{
    // Destruir image views
    for (auto imageView : m_Context.swapChainImageViews) {
        if (imageView != VK_NULL_HANDLE){
            vkDestroyImageView(m_Context.device, imageView, nullptr);
        }
    }

    // Destruir swapchain
    vkDestroySwapchainKHR(m_Context.device, m_Context.swapchain, nullptr);




}


void VulkanSwapchain::destroySwapchain(){
    
    for (auto imageView : m_Context.swapChainImageViews) {
        if (imageView != VK_NULL_HANDLE){
            vkDestroyImageView(m_Context.device, imageView, nullptr);
        }
    }

    // Destruir swapchain
    vkDestroySwapchainKHR(m_Context.device, m_Context.swapchain, nullptr);

};

void VulkanSwapchain::RecreateSwapchain()
{
    std::cout << "\033[1;36m[!] Recreando Swapchain...\033[0m\n";


    int width = 0, height = 0;
    glfwGetFramebufferSize(m_Context.GLFWwindow, &width, &height);
    while (width == 0 || height == 0) {
        glfwWaitEvents();
        glfwGetFramebufferSize(m_Context.GLFWwindow, &width, &height);
    }

    vkDeviceWaitIdle(m_Context.device);
    cleanSwapchain();

    CreateSwapChain();
    CreateImageView();
    //createdepth;

    std::cout << "[+] Swapchain RECREADO - Handle FINAL: " << m_Context.swapchain << std::endl;

    std::cout << "[+] Swapchain recreado correctamente (" 
            << m_Context.swapChainExtent.width << "x" 
            << m_Context.swapChainExtent.height << ")\n";


};

