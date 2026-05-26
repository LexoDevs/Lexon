#include "Buffers.h"

void Pool::createCommandPool(LogicalDevice logicaldevice, PhysicalDevice physicaldevice) {

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    std::cout<<"Hola"<<std::endl;

    auto queueFamilyIndextemp = logicaldevice.findQueueFamilies(physicaldevice.GetPhysicalDevice()).graphicsFamily.value();
    poolInfo.queueFamilyIndex = queueFamilyIndextemp;

    if (vkCreateCommandPool(logicaldevice.GetLogicalDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
    }
    std::cout<<"comand pool creado"<<std::endl;

}

void Pool::destroyCommandPool(LogicalDevice logicaldevice) {

    vkDestroyCommandPool(logicaldevice.GetLogicalDevice(), commandPool, nullptr);

}

void Pool::createCommandBuffer(LogicalDevice logicaldevice){

    VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(logicaldevice.GetLogicalDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}


void transition_image_layout(
	    uint32_t                imageIndex,
	    VkImageLayout         old_layout,
	    VkImageLayout         new_layout,
	    VkAccessFlags2        src_access_mask,
	    VkAccessFlags2        dst_access_mask,
	    VkPipelineStageFlags2 src_stage_mask,
	    VkPipelineStageFlags2 dst_stage_mask,
        Swapchain swapchain,
         VkCommandBuffer         commandBuffer) 
{

		VkImageMemoryBarrier2 barrier = {};
		    barrier.srcStageMask        = src_stage_mask;
		    barrier.srcAccessMask       = src_access_mask;
		    barrier.dstStageMask        = dst_stage_mask;
		    barrier.dstAccessMask       = dst_access_mask;
		    barrier.oldLayout           = old_layout;
		    barrier.newLayout           = new_layout;
		    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            


		    barrier.image               = swapchain.getSwapchainImages()[imageIndex];


		    barrier.subresourceRange    = {
		           .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT ,
		           .baseMipLevel   = 0,
		           .levelCount     = 1,
		           .baseArrayLayer = 0,
		           .layerCount     = 1};
		
        VkDependencyInfo dependency_info = {};
		    dependency_info.dependencyFlags         = {};
		    dependency_info.imageMemoryBarrierCount = 1;
		    dependency_info.pImageMemoryBarriers    = &barrier;

    vkCmdPipelineBarrier2(commandBuffer, &dependency_info);
}

void Pool::recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline)
{
    VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
    

    // Before starting rendering, transition the swapchain image to vk::ImageLayout::eColorAttachmentOptimal
    transition_image_layout(
        imageIndex,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        0,                                              // src_access_mask
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,         // dst_access_mask
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,// src_stage_mask
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,// dst_stage_mask,
        swapchain,
        commandBuffer
        );

        // ====================== INICIO DEL RENDERING ======================
    VkClearValue clearColor{};
    clearColor.color = { {0.0f, 0.0f, 0.0f, 1.0f} };

    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment.imageView   = swapchain.getSwapchainImageView()[imageIndex];  // Asume que tienes este método
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue  = clearColor;

    VkRenderingInfo renderingInfo{};
    renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset    = {0, 0};
    renderingInfo.renderArea.extent    = swapchain.getExtentSwapchain();   // Asume que tienes este getter
    renderingInfo.layerCount           = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments    = &colorAttachment;

    vkCmdBeginRendering(commandBuffer, &renderingInfo);

    // Bind del pipeline gráfico
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGrapicsPipeline());

    // Viewport y Scissor (dynamic state)
    VkViewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = static_cast<float>(swapchain.getExtentSwapchain().width);
    viewport.height   = static_cast<float>(swapchain.getExtentSwapchain().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.getExtentSwapchain();

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Dibujar el triángulo
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    // ====================== FIN DEL RENDERING ======================
    vkCmdEndRendering(commandBuffer);

    // Transition final a Present
    transition_image_layout(
        imageIndex,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,         // src_access_mask
        0,                                              // dst_access_mask
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,// src_stage_mask
        VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,         // dst_stage_mask
        swapchain,
        commandBuffer
    );

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
