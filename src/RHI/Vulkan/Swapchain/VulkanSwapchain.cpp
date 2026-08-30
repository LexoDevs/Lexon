#include "VulkanSwapchain.h"
#include <iostream>
#include <VulkanFormats.h>

VulkanSwapchain::VulkanSwapchain(){

};

VulkanSwapchain::~VulkanSwapchain(){
DestroySwapchain();

};

void VulkanSwapchain::DestroySwapchain(){
    for (auto imageView : swapChainImageViews) {
        if (imageView != VK_NULL_HANDLE){
            vkDestroyImageView(cp_device, imageView, nullptr);
        }
    }
    
    if (swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(cp_device, swapchain, nullptr);
        swapchain = VK_NULL_HANDLE;
    }
}

void VulkanSwapchain::CreateSwapChain(VkDevice device, VkSurfaceKHR surface, SwapchainSupportDetails details, QueueFamilyIndices QueueFamilie ) 
{
    cp_device = device;

    std::cout << "\033[1;36m[!] Creando cadena de intercambio...\033[0m\n";

    swapChainExtent = details.capabilities.currentExtent;

    swapChainSurfaceFormat = details.format;

    uint32_t imageCount = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
        imageCount = details.capabilities.maxImageCount;
    }


    // Crear Swapchain
    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType              = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface            = surface;
    swapChainCreateInfo.minImageCount      = imageCount;
    swapChainCreateInfo.imageFormat        = swapChainSurfaceFormat.format;
    swapChainCreateInfo.imageColorSpace    = swapChainSurfaceFormat.colorSpace;


    VkPresentModeKHR presentationmode = details.presentMode;

    swapChainCreateInfo.imageExtent        = swapChainExtent;
    swapChainCreateInfo.imageArrayLayers   = 1;
    swapChainCreateInfo.imageUsage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.preTransform       = details.capabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha     = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode        = presentationmode;
    swapChainCreateInfo.clipped            = VK_TRUE;


    QueueFamilyIndices indices = QueueFamilie;

    //std::cout << "\033[1;36m[!] 2\033[0m"<<indices.graphicsFamily.value()<<"\n";

    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(),
                                      indices.presentFamily.value() };
    

    if (QueueFamilie.graphicsFamily != QueueFamilie.presentFamily) {
        swapChainCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
    } else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    // Crear el swapchain
    if (vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &swapchain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::cout << "\t\033[1;32mSwapchain\033[0m creada correctamente en \033[1;32m" << swapchain << "\033[0m\n";

    // Obtener las imágenes
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

    swapChainImageFormat = swapChainSurfaceFormat.format;

    std::cout << "\t\033[1;33mSwapchain creado con \033[0m\033[1;32m" << imageCount << "\033[0m\033[1;33m imagenes\033[0m\n";

    for (int i = 0; i<imageCount; i++){
        std::cout<< "\t\t\033[1;33m Vista "<<i+1<<" guardada en \033[0m\033[1;32m" << swapchainImages[i]<<"\n";
    }
}


void VulkanSwapchain::CreateImageView()
{
    swapChainImageViews.resize(swapchainImages.size());

    std::cout << "\033[1;36m[!] Creando vista de imagen\033[0m\n";


    for (size_t i = 0; i < swapchainImages.size(); i++) {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = swapChainSurfaceFormat.format;
        imageViewCreateInfo.image = swapchainImages[i];
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(cp_device, &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
};

/*
void VulkanSwapchain::RecreateSwapchain()
{
    std::cout << "\033[1;36m[!] Recreando Swapchain...\033[0m\n";

    glfwGetFramebufferSize(GLFWwindow, &prop.width, &prop.height);
    while (prop.width == 0 || prop.height == 0) {
        glfwWaitEvents();
        glfwGetFramebufferSize(GLFWwindow, &prop.width, &prop.height);
    }

    vkDeviceWaitIdle(device);
    cleanSwapchain();

    for (auto imageView : swapChainImageViews) {
        if (imageView != VK_NULL_HANDLE) {
            vkDestroyImageView(device, imageView, nullptr);
        }
    }

    swapChainExtent = VkExtent2D(prop.width,prop.height);
    swapChainExtent.width = prop.width;
    swapChainExtent.height = prop.height;

    // Destruir Image Views del swapchain

    swapChainImageViews.clear();

    CreateSwapChain();
    CreateImageView();



    std::cout << "[+] Swapchain RECREADO - Handle FINAL: " << swapchain << std::endl;

    std::cout << "[+] Swapchain recreado correctamente (" 
            << swapChainExtent.width << "x" 
            << swapChainExtent.height << ")\n";


};*/

