#include "VulkanFence.h"
#include <iostream>


void VulkanFence::createSyncObjects()
{
    std::cout << "Creando sincronización para " << MAX_FRAMES_IN_FLIGHT << " frames...\n";

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {   
        VkResult r1 = vkCreateSemaphore(m_Context.device, &semaphoreInfo, nullptr, &m_Context.imageAvailableSemaphore[i]);
        VkResult r2 = vkCreateSemaphore(m_Context.device, &semaphoreInfo, nullptr, &m_Context.renderFinishedSemaphore[i]);
        VkResult r3 = vkCreateFence(m_Context.device, &fenceInfo, nullptr, &m_Context.inFlightFence[i]);

        if (r1 != VK_SUCCESS || r2 != VK_SUCCESS || r3 != VK_SUCCESS)
            throw std::runtime_error("Failed creating sync objects");
    }

    std::cout << "Sincronizacion creada (imageAvailable + Fences)\n";

    

    for (uint32_t i = 0; i < MAX_SWAPCHAIN_IMAGES; i++) {
    }

}

void VulkanFence::destroyFences() {

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (m_Context.imageAvailableSemaphore[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(m_Context.device, 
                               m_Context.imageAvailableSemaphore[i], nullptr);
            m_Context.imageAvailableSemaphore[i] = VK_NULL_HANDLE;
        }

        if (m_Context.renderFinishedSemaphore[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(m_Context.device, 
                               m_Context.renderFinishedSemaphore[i], nullptr);
            m_Context.renderFinishedSemaphore[i] = VK_NULL_HANDLE;
        }

        if (m_Context.inFlightFence[i] != VK_NULL_HANDLE)
        {
            vkDestroyFence(m_Context.device, 
                           m_Context.inFlightFence[i], nullptr);
            m_Context.inFlightFence[i] = VK_NULL_HANDLE;
        }
    }

    
};

void VulkanFence::cleanSync(){
   for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

    m_Context.imageAvailableSemaphore[i]= VK_NULL_HANDLE;
    m_Context.renderFinishedSemaphore[i]= VK_NULL_HANDLE;
    m_Context.inFlightFence[i]= VK_NULL_HANDLE;
   }
};
