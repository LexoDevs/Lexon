#include "Swapchain.h"



void Swapchain::CreateSwapChain(WindowSurface windowsurface, PhysicalDevice physicaldevice, Window  window, LogicalDevice logicaldevice) {
    std::cout << "[!] Creando cadena de intercambio...";

    //Seleccion de capabilities
    VkSurfaceCapabilitiesKHR surfaceCapabilities = windowsurface.GetSurfaceCapabilities(physicaldevice);
    swapChainExtent = windowsurface.chooseSwapExtent(surfaceCapabilities, window);
    
    //Seleccion del formato de superficie
    uint32_t pSurfaceFormatCount;
    std::vector<VkSurfaceFormatKHR> availableFormats = windowsurface.getSurfaceFormats(physicaldevice, pSurfaceFormatCount);
        std::cout<<"Hola\n";

    swapChainSurfaceFormat = windowsurface.chooseSwapSurfaceFormat(availableFormats, pSurfaceFormatCount);
   
    //Seleccion del modo de presentacion
    VkPresentModeKHR presentationmode =  windowsurface.GetSurfacePresentationsMode(physicaldevice);

    //Seleccion del minimo de imagenes
    uint32_t imageCount = surfaceCapabilities.minImageCount+1;
    auto minImageCount = std::max(3u, surfaceCapabilities.minImageCount);
    if ((0 < surfaceCapabilities.maxImageCount) && (surfaceCapabilities.maxImageCount < minImageCount))
    {
        minImageCount = surfaceCapabilities.maxImageCount;
    }

    //CreateSwapchain
    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = windowsurface.getSurface();
    swapChainCreateInfo.imageExtent = getExtentSwapchain();
    swapChainCreateInfo.minImageCount = minImageCount;
    swapChainCreateInfo.imageFormat = swapChainSurfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = swapChainSurfaceFormat.colorSpace;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentationmode;
    swapChainCreateInfo.clipped = true;

    QueueFamilyIndices indices = logicaldevice.findQueueFamilies(physicaldevice.GetPhysicalDevice());
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;
        swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    }

    std::cout<<"Buscando error: "<<logicaldevice.GetLogicalDevice()<<std::endl; 
    std::cout<<"Buscando error: "<<&swapChainCreateInfo<<std::endl; 
    std::cout<<"Buscando error: "<<&swapChain<<std::endl; 

        if (vkCreateSwapchainKHR(logicaldevice.GetLogicalDevice(), &swapChainCreateInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }
    

        std::cout<<"Swapchain creada en: " <<swapChain <<"\n";


    vkGetSwapchainImagesKHR(logicaldevice.GetLogicalDevice(), swapChain, &imageCount, nullptr);
    getSwapchainImages().resize(imageCount);
    vkGetSwapchainImagesKHR(logicaldevice.GetLogicalDevice(), swapChain, &imageCount, getSwapchainImages().data());


    swapChainImageFormat = windowsurface.chooseSwapSurfaceFormat(availableFormats, pSurfaceFormatCount).format;
    swapChainExtent = windowsurface.chooseSwapExtent(surfaceCapabilities, window);
    
};

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

void Swapchain::cleanSwapchain(){
    swapChainImageViews.clear();
    swapChain = nullptr;
}


void Swapchain::destroySwapchain(LogicalDevice logicaldevice){
    

    for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(logicaldevice.GetLogicalDevice(), imageView, nullptr);
        }
    vkDestroySwapchainKHR(logicaldevice.GetLogicalDevice(), swapChain, nullptr);




};

void Swapchain::RecreateSwapchain(LogicalDevice logicaldevices,WindowSurface windowsurface,PhysicalDevice physicaldevice,Window window){

    vkDeviceWaitIdle(logicaldevices.GetLogicalDevice());

    cleanSwapchain();

    CreateSwapChain(windowsurface, physicaldevice, window, logicaldevices);
	CreateImageView(logicaldevices);

};

