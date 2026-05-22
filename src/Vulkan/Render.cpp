#include "Render.h"

void Render::createSyncObjects(LogicalDevice logicaldevice)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;   // Importante: empieza señalizado

    if (vkCreateSemaphore(logicaldevice.GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(logicaldevice.GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(logicaldevice.GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create synchronization objects!");
    }
}

void Render::destroyFences(LogicalDevice logicaldevice) {
    vkDestroySemaphore(logicaldevice.GetLogicalDevice(), imageAvailableSemaphore, nullptr);
    vkDestroySemaphore(logicaldevice.GetLogicalDevice(), renderFinishedSemaphore , nullptr);
    vkDestroyFence(logicaldevice.GetLogicalDevice(), inFlightFence , nullptr);

}

void Render::drawFrame(LogicalDevice logicaldevice, Pool pool, Swapchain swapchain, GraphicsPipeline pipeline)
{
    // PASO 1: Esperar a que el frame anterior termine (CPU-GPU sync)
    std::cout<<"Intentando cargar el logical device:" <<logicaldevice.GetLogicalDevice() <<"\n";
    vkWaitForFences(logicaldevice.GetLogicalDevice(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(logicaldevice.GetLogicalDevice(), 1, &inFlightFence);

    // PASO 2: Adquirir la siguiente imagen del swapchain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        logicaldevice.GetLogicalDevice(),
        swapchain.getSwapchain(),           // tu VkSwapchainKHR
        UINT64_MAX,
        imageAvailableSemaphore,            // semaphore a signalizar cuando la imagen esté lista
        VK_NULL_HANDLE,
        &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {

        //return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // PASO 3: Grabar los comandos en el command buffer
    pool.recordCommandBuffer(imageIndex,swapchain, pipeline);

    // PASO 4: Preparar el Submit (enviar trabajo a la GPU)
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submitInfo{};
    submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = &imageAvailableSemaphore;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;

    auto comand = pool.getCommandBuffer(); 
    submitInfo.pCommandBuffers      = &comand;           // tu command buffer
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &renderFinishedSemaphore;

    if (vkQueueSubmit(logicaldevice.getGraphicQueue(), 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    // PASO 5: Presentar la imagen en pantalla
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = &renderFinishedSemaphore;
    presentInfo.swapchainCount     = 1;

    auto swapt = swapchain.getSwapchain();
    presentInfo.pSwapchains        = &swapt;
    presentInfo.pImageIndices      = &imageIndex;

    result = vkQueuePresentKHR(logicaldevice.getPresentQueue(), &presentInfo);

        frameIndex = (frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

        
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        // Recrear swapchain
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }



}
