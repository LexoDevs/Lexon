#include "VulkanRHI.h"
#include <iostream>

 #include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h" 


VulkanRHI::VulkanRHI()
    : context()                    // Primero creamos el contexto
    , window(context)
    , instance(context)
    , validacionlayers(context)
    , surface(context)
    , physicaldevice(context)
    , device(context)
    , swapchain(context)
    , pipeline(context)
    , commandpool(context)
    , descriptorpool(context)
    , fences(context)
    , depthBuffer(context)
    , texture(context)
    , vertexBuffer(context)
    , indexBuffer(context)
    , uniformBuffer(context)
    , descriptorSet(context)
    , commandBuffer(context)

{
}

    void VulkanRHI::InitVulkan(){ 
        instance.CreateInstances();
        validacionlayers.setupDebugMessenger();
        surface.CreateWindowSurface();
        physicaldevice.SelectPhysicalDevices();
        surface.GetSurfaceCapabilities();
        surface.GetSurfacePresentationsMode();
        surface.chooseSwapSurfaceFormat();
	    device.CreateLogicalDevice();
        swapchain.CreateSwapChain();
        swapchain.CreateImageView();

        depthBuffer.createDepthResources();
        
        

        pipeline.CreateDescriptorSetLayout();
        pipeline.createGraphicsPipeline(); 

        commandpool.createCommandPool(); 
        texture.createTextureImage();
        texture.createTextureImageView(VK_IMAGE_ASPECT_DEPTH_BIT);
        texture.createTextureSampler();
    };

    void VulkanRHI::InitPostLoadElements(ObjectInstance& mesh){
        vertexBuffer.createVertexBuffer(mesh);
        indexBuffer.createIndexBuffer(mesh);
        uniformBuffer.createUniformBuffer();
        descriptorpool.createDescriptorPool();
        descriptorSet.createDescriptorSets();
        commandBuffer.createCommandBuffer();
        fences.createSyncObjects();
    };




    void VulkanRHI::DestroyVulkan(){ 
        fences.destroyFences();
        descriptorSet.destroyDescriptorSet();
descriptorpool.destroyDescriptorPool();
uniformBuffer.destroyUniformBuffer();
        indexBuffer.destroyIndexBuffer();
        vertexBuffer.destroyVertexBuffer();

texture.destroyImageTexture();
texture.destroyImageTextureView();

        depthBuffer.destroyDepthResources();
        commandpool.destroyCommandPool();
        pipeline.DestroyDescriptorSetLayout();

        swapchain.destroySwapchain();
        physicaldevice.DestroyPhysicalDevices();
        surface.DestroyVulkanSurface();
        validacionlayers.DestroyValidationLayers();
        instance.DestroyInstance();
        
    };

    void VulkanRHI::DrawFrame( CameraView& camera,ObjectInstance& mesh){


//std::cout<<"Dentro del draw, esperando frames"<<std::endl;
    // 1. Esperar al frame anterior
   vkDeviceWaitIdle(context.device);    //Solucion del error del index



    vkWaitForFences(context.device, 1, 
                    &context.inFlightFence[context.frameIndex], VK_TRUE, UINT64_MAX);


    // 2. Adquirir imagen del swapchain

    uint32_t imageIndex;
    //std::cout<<"swapchain: "<<context.swapchain<<std::endl;


    VkResult result = vkAcquireNextImageKHR(
        context.device,
        context.swapchain,
        UINT64_MAX,
        context.imageAvailableSemaphore[context.frameIndex],
        VK_NULL_HANDLE,
        &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {

        vkDeviceWaitIdle(context.device);

        swapchain.RecreateSwapchain();

        depthBuffer.cleanDepthResources();
        depthBuffer.createDepthResources();
        
        pipeline.recreateGraphicsPipeline();

        return;   // Salir y reintentar en el próximo ciclo
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }



    uniformBuffer.updateUniformBuffer(context.frameIndex, mesh, camera);
    // 3. Resetear fence
    vkResetFences(context.device, 1, &context.inFlightFence[context.frameIndex]);

        // 4. Grabar comandos
    vkResetCommandBuffer(context.commandBuffers[context.frameIndex], 0);

    VkCommandBuffer cmd = context.commandBuffers[context.frameIndex];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    //vkBeginCommandBuffer(cmd, &beginInfo);

    // Tu renderizado normal
    depthBuffer.recordCommandBuffer(imageIndex, context.frameIndex, mesh);



    // 5. Submit
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {context.imageAvailableSemaphore[context.frameIndex]};
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;

    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &context.commandBuffers[context.frameIndex];

    VkSemaphore signalSemaphores[] = {context.renderFinishedSemaphore[context.frameIndex]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;  



    if (vkQueueSubmit(context.graphicsQueue, 1, &submitInfo, 
                      context.inFlightFence[context.frameIndex]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }


    // 6. Present
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = {context.swapchain};
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains   = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(context.graphicsQueue, &presentInfo); // Posible error, tengo dudas

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || context.framebufferResized) {
        std::cout<<"Post Error"<<std::endl;

        context.framebufferResized = false;
        vkDeviceWaitIdle(context.device);

        swapchain.RecreateSwapchain();

        depthBuffer.cleanDepthResources();
        depthBuffer.createDepthResources();
        
        pipeline.recreateGraphicsPipeline();
    } 
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    // Avanzar al siguiente frame
    context.frameIndex  = (context.frameIndex  + 1) % MAX_FRAMES_IN_FLIGHT;

    };

    void VulkanRHI::InitWindowSistem(){
        window.InitWindowsSistem();
    };

    void VulkanRHI::DestroyWindowSistem(){
        window.DestroyWindowsSistem();
    };
