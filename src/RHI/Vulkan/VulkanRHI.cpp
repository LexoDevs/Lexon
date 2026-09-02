#include "VulkanRHI.h"
#include <iostream>

 #include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h" 

#include <iostream>

VulkanRHI::VulkanRHI()
    : context()
    // Primero creamos el contexto
    , pipeline(context)
    , depthBuffer(context)
    , texture(context)
    , uniformBuffer(context)
    , descriptorSet(context)


{
}

void VulkanRHI::InitVulkan(Window& window)
{
    activeWindow = &window;
    // Instancia de vulkan
    instance.CreateInstances();

    // Superficie de Ventana
    surface.CreateWindowSurface(
        window.GetNativeWindow(),
        instance.GetInstance()
    );

    // Seleccion de GPU
    physicaldevice.SelectPhysicalDevices(
        instance.GetInstance()
    );

    // Buscar capacidades de la grafica y la ventana
    physicaldevice.CreateSwapchainSupportDetails(
        surface.GetSurface()
    );

    // Crear dispositivo virtual y colas (Queues)
    device.CreateLogicalDevice(
        physicaldevice.GetPhysicalDevice()
    );

    commandpool.createCommandPool(
        device.GetHandle(),
        device.GetQueueFamily()
    ); 

    swapchain.CreateSwapChain(
        device.GetHandle(),
        surface.GetSurface(),
        physicaldevice.GetSwapDetails(),
        device.GetQueueFamily()
    );

    swapchain.CreateImageView();

    fences.createSyncObjects(
        device.GetHandle()
    );
            std::cout<<device.GetHandle()<<std::endl;

    commandBuffers.createCommandBuffer(
        device.GetHandle(),
        commandpool.GetHandle());

            std::cout<<device.GetHandle()<<std::endl;

    };


    void VulkanRHI::InitRenderer(){
/*
    depthBuffer.createDepthResources();

    pipeline.CreateDescriptorSetLayout();
    pipeline.createGraphicsPipeline();

    uniformBuffer.createUniformBuffer();

*/
    descriptorpool.createDescriptorPool(device.GetHandle());
    layerdescriptorpool.CreateImGuiDescriptorPool(device.GetHandle());

/*    
    descriptorSet.createDescriptorSets();

    texture.createTextureImage();
    texture.createTextureImageView();
    texture.createTextureSampler();
*/
    };

void VulkanRHI::UploadMesh(CpuModel mesh){
    

    vertexBuffer.createVertexBuffer(
        mesh,
        GetVulkanLogicalDevice().GetHandle(),
        physicaldevice.GetPhysicalDevice(),
        commandpool.GetHandle(),
        device.GetGraphicsQueue(),
    commandBuffers.GetCommandBuffer(0));


    indexBuffer.createIndexBuffer(
        mesh,
        GetVulkanLogicalDevice().GetHandle(),
        physicaldevice.GetPhysicalDevice(),
        commandpool.GetHandle(),
        device.GetGraphicsQueue(),
    commandBuffers.GetCommandBuffer(0));;
    std::cout<<"De locos"<<std::endl;

};




void VulkanRHI::DestroyVulkan(){ 


    pipeline.DestroyPipelineGraphics();

    uniformBuffer.destroyUniformBuffer();

    descriptorpool.destroyDescriptorPool();

    texture.destroyImageTexture();
    texture.destroyImageTextureView();
    depthBuffer.destroyDepthResources();

    descriptorSet.destroyDescriptorSet();

    pipeline.DestroyDescriptorSetLayout();

    indexBuffer.destroyIndexBuffer();

    vertexBuffer.destroyVertexBuffer();
    



};

void VulkanRHI::DrawFrame( CameraView& camera,std::vector<CpuMesh> mesh, bool& UIVis){
    const uint32_t frame = currentFrame;
    
    //0. Inicializar recursos

    VkDevice vkDevice = device.GetHandle();

    VkFence fence = fences.GetinFlightFence(frame);

    VkSemaphore imageAvailable = fences.GetimageAvailableSemaphore(frame);


    VkCommandBuffer commandBuffer = commandBuffers.GetCommandBuffer(frame);

    //std::cout<<"Dentro del draw, esperando frames"<<std::endl;
   //vkDeviceWaitIdle(context.device);    //Solucion del error del index

    // 1. Esperar al frame anterior
    vkWaitForFences(device.GetHandle(), 1, &fence, VK_TRUE, UINT64_MAX);

    if (activeWindow != nullptr && activeWindow->WindowWasResiced()) {
        RecreateSwapchain(activeWindow);
        return;
    }

    // 2. Adquirir imagen del swapchain
    uint32_t imageIndex;

    VkResult result = vkAcquireNextImageKHR(
        vkDevice,
        swapchain.GetSwapchain(),
        UINT64_MAX,
        imageAvailable,
        VK_NULL_HANDLE,
        &imageIndex);

   if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapchain(activeWindow);
        return;
        //window.framebufferResized = true;  // Para que se redimensione en el próximo frame
    }
    
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    const bool acquireWasSuboptimal = (result == VK_SUBOPTIMAL_KHR);
    VkSemaphore renderFinished = fences.GetrenderFinishedSemaphore(imageIndex);


    //uniformBuffer.updateUniformBuffer(context.frameIndex, mesh, camera);
    
    // 3. Resetear fence
    vkResetFences(vkDevice, 1, &fence);

    // 4. Grabar comandos
    vkResetCommandBuffer(commandBuffer, 0);



//Aqui empieza el grabado de comandos a la gráfica
    recordCommandBuffer(frame,imageIndex, mesh, UIVis);



    // 5. Submit
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemaphores[] = {imageAvailable};
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;

    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &commandBuffer;

    VkSemaphore signalSemaphores[] = {renderFinished};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;  


    if (vkQueueSubmit(device.GetGraphicsQueue(), 1, &submitInfo, fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }


    // 6. Present
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain.GetSwapchain()};
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains   = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(device.GetGraphicsQueue(), &presentInfo); // Posible error, tengo dudas

    if (result != VK_SUCCESS &&
        result != VK_ERROR_OUT_OF_DATE_KHR &&
        result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    if (result == VK_ERROR_OUT_OF_DATE_KHR ||
        result == VK_SUBOPTIMAL_KHR ||
        acquireWasSuboptimal ||
        (activeWindow != nullptr && activeWindow->WindowWasResiced())) {
        RecreateSwapchain(activeWindow);
    }
    else {
        // Avanzar al siguiente frame solo si conservamos el swapchain actual.
        currentFrame  = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
};


void VulkanRHI::recordCommandBuffer(uint32_t frame, uint32_t imageIndex, std::vector<CpuMesh> mesh, bool& UIVisibility)
{
    VkCommandBuffer cmd = commandBuffers.GetCommandBuffer(frame);



    VkImage swapchainimages = swapchain.GetSwapchainImages(imageIndex);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    

    transition_image_layout(
        cmd,
        swapchainimages,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,                                                // srcAccessMask
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,             // srcStage
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT  // dstStage
    );
    
    /*
    // Transición del Depth Image
    transition_image_layout(
        cmd,
        m_Context.depthImage,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
        VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
    );
*/
        // ====================== INICIO DEL RENDERING ======================
    VkClearValue clearValues[1] = {};
    clearValues[0].color = { {0.5f, 0.5f, 0.5f, 1.0f} };
    //clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment.imageView   = swapchain.GetSwapChainImageViews(imageIndex);
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue  = clearValues[0];


/*
    VkRenderingAttachmentInfo depthAttachment{};
    depthAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.imageView   = m_Context.depthImageView;


    depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    depthAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.clearValue  = clearValues[1];
*/

    VkRenderingInfo renderingInfo{};
    renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset    = {0, 0};
    renderingInfo.renderArea.extent    = {swapchain.GetSwapchainExtent().width,swapchain.GetSwapchainExtent().height};
    renderingInfo.layerCount           = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments    = &colorAttachment;
    //renderingInfo.pDepthAttachment     = &depthAttachment;

    

    vkCmdBeginRendering(cmd, &renderingInfo);

    // Bind del pipeline gráfico
    //vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Context.Pipeline);

    vkCmdSetDepthTestEnable(cmd, VK_TRUE);
vkCmdSetDepthWriteEnable(cmd, VK_TRUE);
vkCmdSetDepthCompareOp(cmd, VK_COMPARE_OP_LESS);

vkCmdSetDepthBiasEnable(cmd, VK_FALSE);
vkCmdSetDepthBias(cmd, 0.0f, 0.0f, 0.0f);

vkCmdSetStencilTestEnable(cmd, VK_FALSE);

vkCmdSetDepthBoundsTestEnable(cmd, VK_FALSE);
vkCmdSetDepthBounds(cmd, 0.0f, 1.0f);

    // Viewport y Scissor (dynamic state)
    VkViewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = static_cast<float>(1280);//window.width
    viewport.height   = static_cast<float>(720);//window.height
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    // Scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.GetSwapchainExtent();

    vkCmdSetViewport(cmd, 0, 1, &viewport); //??
    vkCmdSetScissor(cmd, 0, 1, &scissor);


    VkBuffer vertexBuffers =  vertexBuffer.GetBuffer() ;
    VkDeviceSize offsets[]   = { 0 };
    
    vkCmdBindVertexBuffers(cmd, 0, 1, &vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmd, indexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

    /*vkCmdBindDescriptorSets(cmd, 
                        VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        m_Context.pipelineLayout, 
                        0, 1, 
                        &m_Context.descriptorSets[frame],   // Asumiendo que es un método de Texture
                        0, nullptr);
*/
mesh[0].indices.size();
    vkCmdDrawIndexed(cmd, 
    static_cast<uint32_t>(mesh[0].indices.size()),
     1, 
     0, 
     0,
     0);


if ( UIVisibility == true){
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

}
//std::cout<<UIVisibility<<std::endl;;

    vkCmdEndRendering(cmd);


    transition_image_layout(
        cmd,
        swapchainimages,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        0,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT
    );

    if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}


/**/
void VulkanRHI::RecreateSwapchain(Window* window)
{
    std::cout << "\033[1;36m[!] Recreando Swapchain...\033[0m\n";

    if (activeWindow == nullptr) {
        throw std::runtime_error("Cannot recreate swapchain without a window");
    }

    while (activeWindow->GetWidth() == 0 || activeWindow->GetHeight() == 0) {
        activeWindow->WaitEvents();
    }

    VkDevice vkDevice = device.GetHandle();
    vkDeviceWaitIdle(vkDevice);

    fences.destroyFences();
    swapchain.DestroySwapchain();

    physicaldevice.CreateSwapchainSupportDetails(surface.GetSurface());

    swapchain.CreateSwapChain(
        vkDevice,
        surface.GetSurface(),
        physicaldevice.GetSwapDetails(),
        device.GetQueueFamily()
        );
    swapchain.CreateImageView();

    fences.createSyncObjects(vkDevice);

    if (ImGui::GetCurrentContext() != nullptr) {
        //ImGui_ImplVulkan_SetMinImageCount(swapchain.GetMinImageCount());
    }

    currentFrame = 0;
    activeWindow->ResetResizeFlag();



    std::cout << "[+] Swapchain RECREADO - Handle FINAL: " << swapchain.GetSwapchain() << std::endl;

    std::cout << "[+] Swapchain recreado correctamente (" 
            << swapchain.GetSwapchainExtent().width << "x" 
            << swapchain.GetSwapchainExtent().height << ")\n";


};