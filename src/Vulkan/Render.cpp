

#include "Render.h"



void Render::createSyncObjects(LogicalDevice logicaldevice)
{
    std::cout << "Creando sincronización para " << MAX_FRAMES_IN_FLIGHT << " frames...\n";

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {   
        VkResult r1 = vkCreateSemaphore(logicaldevice.GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]);
        VkResult r2 = vkCreateSemaphore(logicaldevice.GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]);
        VkResult r3 = vkCreateFence(logicaldevice.GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFence[i]);

        if (r1 != VK_SUCCESS || r2 != VK_SUCCESS || r3 != VK_SUCCESS)
            throw std::runtime_error("Failed creating sync objects");
    }

    std::cout << "Sincronizacion creada (imageAvailable + Fences)\n";

    

    for (uint32_t i = 0; i < MAX_SWAPCHAIN_IMAGES; i++) {
    }

}

void Render::destroyFences(LogicalDevice logicaldevice) {

    for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (imageAvailableSemaphore[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(logicaldevice.GetLogicalDevice(), 
                               imageAvailableSemaphore[i], nullptr);
            imageAvailableSemaphore[i] = VK_NULL_HANDLE;
        }

        if (renderFinishedSemaphore[i] != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(logicaldevice.GetLogicalDevice(), 
                               renderFinishedSemaphore[i], nullptr);
            renderFinishedSemaphore[i] = VK_NULL_HANDLE;
        }

        if (inFlightFence[i] != VK_NULL_HANDLE)
        {
            vkDestroyFence(logicaldevice.GetLogicalDevice(), 
                           inFlightFence[i], nullptr);
            inFlightFence[i] = VK_NULL_HANDLE;
        }
    }

    
};

void Render::cleanSync(LogicalDevice logicaldevice){
   for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

    imageAvailableSemaphore[i]= VK_NULL_HANDLE;
    renderFinishedSemaphore[i]= VK_NULL_HANDLE;
    inFlightFence[i]= VK_NULL_HANDLE;
   }
};



void Render::drawFrame(LogicalDevice logicaldevice, Pool pool, Swapchain& swapchain,
                       GraphicsPipeline pipeline, WindowSurface windowsurface,
                       PhysicalDevice physicaldevice, Window window, VertexBuffer vertexbuffer, Texture texture,
                    DepthBuffer &depthbuffer,  VkClearValue clearDepth, VkImage depthImage, ObjectInstance mesh, CameraView camera)
{


//std::cout<<"Dentro del draw, esperando frames"<<std::endl;
    // 1. Esperar al frame anterior
   vkDeviceWaitIdle(logicaldevice.GetLogicalDevice());    //Solucion del error del index



    vkWaitForFences(logicaldevice.GetLogicalDevice(), 1, 
                    &inFlightFence[frameIndex], VK_TRUE, UINT64_MAX);


    // 2. Adquirir imagen del swapchain

    uint32_t imageIndex;
    //std::cout<<"swapchain: "<<swapchain.getSwapchain()<<std::endl;


    VkResult result = vkAcquireNextImageKHR(
        logicaldevice.GetLogicalDevice(),
        swapchain.getSwapchain(),
        UINT64_MAX,
        imageAvailableSemaphore[frameIndex],
        VK_NULL_HANDLE,
        &imageIndex);

    //std::cout<<"Imageindex: "<<imageIndex<<std::endl;
    //std::cout<<"frameIndex: "<<frameIndex<<std::endl;

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapchain.RecreateSwapchain(logicaldevice, windowsurface, physicaldevice, window);//0xc6031ff8f0//0xc6031ff740
                depthbuffer.cleanDepthResources(logicaldevice);

        depthbuffer.createDepthResources(physicaldevice,texture, logicaldevice, vertexbuffer, swapchain);
        return;   // Salir y reintentar en el próximo ciclo
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vertexbuffer.updateUniformBuffer(swapchain,frameIndex,mesh, camera);
    // 3. Resetear fence
    vkResetFences(logicaldevice.GetLogicalDevice(), 1, &inFlightFence[frameIndex]);

    // 4. Grabar comandos
    vkResetCommandBuffer(vertexbuffer.getCommandBuffer(frameIndex),0);

    auto depth = depthbuffer.getdepthImage();
    depthbuffer.recordCommandBuffer(imageIndex, swapchain, pipeline, frameIndex, vertexbuffer, texture, clearDepth, depth,mesh);

    // 5. Submit
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore[frameIndex]};
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;

    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &vertexbuffer.getCommandBuffer(frameIndex);

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore[frameIndex]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;  



    if (vkQueueSubmit(logicaldevice.getGraphicQueue(), 1, &submitInfo, 
                      inFlightFence[frameIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }
      //  std::cout<<"Post Error"<<std::endl;


    // 6. Present
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain.getSwapchain()};
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains   = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(logicaldevice.getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.getframebufferResized()) {
        window.getframebufferResized() = false;
        swapchain.RecreateSwapchain(logicaldevice, windowsurface, physicaldevice, window);
        
        depthbuffer.cleanDepthResources(logicaldevice);

        depthbuffer.createDepthResources(physicaldevice,texture, logicaldevice, vertexbuffer, swapchain);
    } 
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    // Avanzar al siguiente frame
    frameIndex  = (frameIndex  + 1) % MAX_FRAMES_IN_FLIGHT;

}


