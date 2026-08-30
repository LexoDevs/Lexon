#include "VulkanFence.h"
#include <iostream>

VulkanFence::VulkanFence(){

};

VulkanFence::~VulkanFence(){
    destroyFences();
};

void VulkanFence::createSyncObjects(VkDevice device)
{
    cp_device = device;
    std::cout << "Creando sincronización para " << MAX_FRAMES_IN_FLIGHT << " frames...\n";

        std::cout
        << "[SYNC CREATE] this=" << this
        << " device=" << device
        << std::endl;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {  

        if (vkCreateSemaphore( device, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i] ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image available semaphore");
        }
        
        std::cout
        << "[SYNC CREATE] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;
        if (vkCreateFence( device, &fenceInfo, nullptr, &inFlightFence[i] ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create in-flight fence");
        }
        std::cout
        << "[SYNC CREATE] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;

    }

    for (uint32_t i = 0; i < MAX_SWAPCHAIN_IMAGES; i++)
    {  
        if (vkCreateSemaphore( device, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i] ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render finished semaphore");
        }
        std::cout
        << "[SYNC CREATE] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;
    
    }
    std::cout << "Sincronizacion creada (imageAvailable + Fences)\n";



}

void VulkanFence::destroyFences() {



    if (cp_device == VK_NULL_HANDLE)
        return;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        std::cout
        << "[SYNC STATE] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;

        if (imageAvailableSemaphore[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(cp_device, imageAvailableSemaphore[i], nullptr);
            imageAvailableSemaphore[i] = VK_NULL_HANDLE;
        }

        std::cout
        << "[SYNC DESTROY] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;

        if (inFlightFence[i] != VK_NULL_HANDLE)
        {
            vkDestroyFence(cp_device, inFlightFence[i], nullptr );
            inFlightFence[i] = VK_NULL_HANDLE;
        }
                std::cout
        << "[SYNC DESTROY] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;
    }

    for (uint32_t i = 0; i < MAX_SWAPCHAIN_IMAGES; ++i)
    {
            std::cout
        << "[SYNC DESTROY] frame " << i
        << " imageAvailable=" << imageAvailableSemaphore[i]
        << " renderFinished=" << renderFinishedSemaphore[i]
        << " fence=" << inFlightFence[i]
        << std::endl;
        if (renderFinishedSemaphore[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(cp_device, renderFinishedSemaphore[i], nullptr);
            renderFinishedSemaphore[i] = VK_NULL_HANDLE;
        }
    }

    std::cout<<"Eliminacion d los semaforos"<<std::endl;
    
    cp_device = VK_NULL_HANDLE; 
};

/*
void VulkanFence::cleanSync(){
   for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

    m_Context.imageAvailableSemaphore[i]= VK_NULL_HANDLE;
    m_Context.renderFinishedSemaphore[i]= VK_NULL_HANDLE;
    m_Context.inFlightFence[i]= VK_NULL_HANDLE;
   }
};
*/