#include "Swapchain.h"

void Swapchain::CreateSwapChain(WindowSurface windowsurface, PhysicalDevice physicaldevice, 
                                Window window, LogicalDevice logicaldevice) 
{
    std::cout << "[!] Creando cadena de intercambio...\n";

    VkSurfaceCapabilitiesKHR surfaceCapabilities = windowsurface.GetSurfaceCapabilities(physicaldevice);
    swapChainExtent = windowsurface.chooseSwapExtent(surfaceCapabilities, window);

    // Formato
    uint32_t pSurfaceFormatCount;
    std::vector<VkSurfaceFormatKHR> availableFormats = windowsurface.getSurfaceFormats(physicaldevice, pSurfaceFormatCount);
    swapChainSurfaceFormat = windowsurface.chooseSwapSurfaceFormat(availableFormats, pSurfaceFormatCount);

    // Present mode
    VkPresentModeKHR presentationmode = windowsurface.GetSurfacePresentationsMode(physicaldevice);

    // Número de imágenes
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // Crear Swapchain
    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType              = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface            = windowsurface.getSurface();
    swapChainCreateInfo.minImageCount      = imageCount;
    swapChainCreateInfo.imageFormat        = swapChainSurfaceFormat.format;
    swapChainCreateInfo.imageColorSpace    = swapChainSurfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent        = swapChainExtent;
    swapChainCreateInfo.imageArrayLayers   = 1;
    swapChainCreateInfo.imageUsage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.preTransform       = surfaceCapabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha     = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode        = presentationmode;
    swapChainCreateInfo.clipped            = VK_TRUE;

    QueueFamilyIndices indices = logicaldevice.findQueueFamilies(physicaldevice.GetPhysicalDevice());
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        swapChainCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices   = queueFamilyIndices;
    } else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    // Crear el swapchain
    if (vkCreateSwapchainKHR(logicaldevice.GetLogicalDevice(), &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    std::cout << "Swapchain creada correctamente - Handle: " << swapChain << "\n";

    // Obtener las imágenes
    vkGetSwapchainImagesKHR(logicaldevice.GetLogicalDevice(), swapChain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicaldevice.GetLogicalDevice(), swapChain, &imageCount, swapchainImages.data());

    swapChainImageFormat = swapChainSurfaceFormat.format;

    std::cout << "Swapchain creado con " << imageCount << " imágenes\n";
}



void Swapchain::CreateImageView(LogicalDevice logicaldevice)
{
    swapChainImageViews.resize(swapchainImages.size());



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

        std::cout<<"Hola: "<<&getSwapchainImageView()[i+1]<<"\n";


        if (vkCreateImageView(logicaldevice.GetLogicalDevice(), &imageViewCreateInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
};

void Swapchain::cleanSwapchain(LogicalDevice logicaldevice)
{
    // Destruir image views
    for (auto imageView : swapChainImageViews) {

        vkDestroyImageView(logicaldevice.GetLogicalDevice(), imageView, nullptr);
    }

    // Destruir swapchain
    vkDestroySwapchainKHR(logicaldevice.GetLogicalDevice(), swapChain, nullptr);




}


void Swapchain::destroySwapchain(LogicalDevice logicaldevice){
    
    for (auto imageView : swapChainImageViews) {

            vkDestroyImageView(logicaldevice.GetLogicalDevice(), imageView, nullptr);
    }

    // Destruir swapchain
    vkDestroySwapchainKHR(logicaldevice.GetLogicalDevice(), swapChain, nullptr);

};

void Swapchain::RecreateSwapchain(LogicalDevice logicaldevice, WindowSurface windowsurface,
                                  PhysicalDevice physicaldevice, Window window)
{
    std::cout << "[+] RECREANDO SWAPCHAIN...\n";

    int width = 0, height = 0;
    glfwGetFramebufferSize(window.GetWindows(0), &width, &height);
    while (width == 0 || height == 0) {
        glfwWaitEvents();
        glfwGetFramebufferSize(window.GetWindows(0), &width, &height);
    }

    vkDeviceWaitIdle(logicaldevice.GetLogicalDevice());
    cleanSwapchain(logicaldevice);

    CreateSwapChain(windowsurface, physicaldevice, window, logicaldevice);
    CreateImageView(logicaldevice);

    std::cout << "[+] Swapchain RECREADO - Handle FINAL: " << swapChain << std::endl;

    std::cout << "[+] Swapchain recreado correctamente (" 
            << getExtentSwapchain().width << "x" 
            << getExtentSwapchain().height << ")\n";


};

