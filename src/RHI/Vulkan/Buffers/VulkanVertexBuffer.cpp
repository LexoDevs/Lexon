
#include "VulkanVertexBuffer.h"


 #include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

void transition_image_layout(
    VkCommandBuffer         commandBuffer,
    VkImage                 &image,
    VkImageLayout           old_layout,
    VkImageLayout           new_layout,
    VkImageAspectFlags      aspectMask,
    VkAccessFlags2          src_access_mask,
    VkAccessFlags2          dst_access_mask,
    VkPipelineStageFlags2   src_stage_mask,
    VkPipelineStageFlags2   dst_stage_mask)
{
    VkImageMemoryBarrier2 barrier{};
    barrier.sType                   = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.srcStageMask            = src_stage_mask;
    barrier.srcAccessMask           = src_access_mask;
    barrier.dstStageMask            = dst_stage_mask;
    barrier.dstAccessMask           = dst_access_mask;
    barrier.oldLayout               = old_layout;
    barrier.newLayout               = new_layout;
    barrier.srcQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex     = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                   = image;

    barrier.subresourceRange = {
        .aspectMask     = aspectMask,
        .baseMipLevel   = 0,
        .levelCount     = 1,
        .baseArrayLayer = 0,
        .layerCount     = 1
    };

    VkDependencyInfo dependency_info{};
    dependency_info.sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.imageMemoryBarrierCount = 1;
    dependency_info.pImageMemoryBarriers    = &barrier;

    vkCmdPipelineBarrier2(commandBuffer, &dependency_info);
}

VkCommandBuffer BufferI::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_Context.commandPool;
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(m_Context.device, &allocInfo, &m_Context.commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(m_Context.commandBuffer, &beginInfo);

    return m_Context.commandBuffer;
}

void BufferI::endSingleTimeCommands() {
    vkEndCommandBuffer(m_Context.commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_Context.commandBuffer;

    vkQueueSubmit(m_Context.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_Context.graphicsQueue);

    vkFreeCommandBuffers(m_Context.device, m_Context.commandPool, 1, &m_Context.commandBuffer);
}


void BufferI::createBuffer(VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    VkBuffer& buffer, VkDeviceMemory& bufferMemory){

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_Context.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(m_Context.device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(m_Context.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(m_Context.device, buffer, bufferMemory, 0);
}

void BufferI::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands();

}

uint32_t BufferI::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        vkGetPhysicalDeviceMemoryProperties(m_Context.physicalDevice, &m_Context.memProperties);

        for (uint32_t i = 0; i < m_Context.memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (m_Context.memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                m_Context.VulkanMemoryTypeIndex = i;
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
}




void VertexBuffer::createVertexBuffer(ObjectInstance& mesh){



    VkDeviceSize bufferSize = sizeof(mesh.getVertices()[0]) * mesh.getVertices().size();
    std::cout<<"Tamano de buffer: "<<bufferSize<<std::endl;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_Context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, mesh.getVertices().data(), (size_t) bufferSize);
    vkUnmapMemory(m_Context.device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context.vertexBuffer, m_Context.vertexBufferMemory);
    copyBuffer(stagingBuffer, m_Context.vertexBuffer, bufferSize);

            vkDestroyBuffer(m_Context.device, stagingBuffer, nullptr);
        vkFreeMemory(m_Context.device, stagingBufferMemory, nullptr);

}

void VertexBuffer::destroyVertexBuffer(){

        vkDestroyBuffer(m_Context.device, m_Context.vertexBuffer, nullptr);
        vkFreeMemory(m_Context.device, m_Context.vertexBufferMemory, nullptr);
        
}

void IndexBuffer::createIndexBuffer(ObjectInstance& mesh){

    VkDeviceSize bufferSize = sizeof(mesh.getIndices()[0]) * mesh.getIndices().size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_Context.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, mesh.getIndices().data(), (size_t) bufferSize);
    vkUnmapMemory(m_Context.device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context.indexBuffer, m_Context.indexBufferMemory);

    copyBuffer(stagingBuffer, m_Context.indexBuffer, bufferSize);

    vkDestroyBuffer(m_Context.device, stagingBuffer, nullptr);
    vkFreeMemory(m_Context.device, stagingBufferMemory, nullptr);

}

void IndexBuffer::destroyIndexBuffer(){
    
        vkDestroyBuffer(m_Context.device, m_Context.indexBuffer, nullptr);
    vkFreeMemory(m_Context.device, m_Context.indexBufferMemory, nullptr);
}




void UniformBuffer::createUniformBuffer(){
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_Context.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    m_Context.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Context.uniformBuffers[i], m_Context.uniformBuffersMemory[i]);

        vkMapMemory(m_Context.device, m_Context.uniformBuffersMemory[i], 0, bufferSize, 0, &m_Context.uniformBuffersMapped[i]);
    }
};

void UniformBuffer::destroyUniformBuffer(){
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(m_Context.device, m_Context.uniformBuffers[i], nullptr);
        vkFreeMemory(m_Context.device, m_Context.uniformBuffersMemory[i], nullptr);
    }

}

void UniformBuffer::updateUniformBuffer(uint32_t currentImage, ObjectInstance mesh, CameraView camera) {
    
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    float aspectratio = m_Context.swapChainExtent.width / (float) m_Context.swapChainExtent.height;


    
    mesh.SetMatrixModel(m_Context.UBO, time, glm::vec3(0.0f, 0.0f, 0.0f));
    camera.SetCameraView(m_Context.UBO, aspectratio, time);

    memcpy(m_Context.uniformBuffersMapped[currentImage], &m_Context.UBO, sizeof(m_Context.UBO));

}




















void DepthBuffer::createDepthFormat(){

        m_Context.depthformat = findDepthFormat();


}

void DepthBuffer::createDepthResources(){
        createDepthFormat();
        
        m_Context.depthsize.height = m_Context.swapChainExtent.height;
        m_Context.depthsize.width = m_Context.swapChainExtent.width;

        createImage(m_Context.depthsize.width, m_Context.depthsize.height, m_Context.depthformat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context.depthImage, m_Context.depthImageMemory);
        m_Context.depthImageView = createImageView(m_Context.depthImage, m_Context.depthformat, VK_IMAGE_ASPECT_DEPTH_BIT );
        
};


VkFormat DepthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_Context.physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                m_Context.depthformat = format;
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                m_Context.depthformat = format;
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
    }


VkFormat DepthBuffer::findDepthFormat() {
    return findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


bool DepthBuffer::hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

void DepthBuffer::destroyDepthResources(){

    vkDestroyImageView(m_Context.device, m_Context.depthImageView, nullptr);
    vkDestroyImage(m_Context.device, m_Context.depthImage, nullptr);
    vkFreeMemory(m_Context.device, m_Context.depthImageMemory, nullptr);
};

void DepthBuffer::cleanDepthResources(){
    vkDestroyImageView(m_Context.device, m_Context.depthImageView, nullptr);
    vkDestroyImage(m_Context.device, m_Context.depthImage, nullptr);
    vkFreeMemory(m_Context.device, m_Context.depthImageMemory, nullptr);

};

void DepthBuffer::recordCommandBuffer(uint32_t imageIndex, uint32_t currentFrame, ObjectInstance mesh)
{
    VkCommandBuffer cmd = m_Context.commandBuffers[currentFrame];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    

    transition_image_layout(
        cmd,
        m_Context.swapchainImages[imageIndex],
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,                                                // srcAccessMask
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,             // srcStage
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT  // dstStage
    );
        
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

        // ====================== INICIO DEL RENDERING ======================
    VkClearValue clearValues[2] = {};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment.imageView   = m_Context.swapChainImageViews[imageIndex];
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue  = clearValues[0];



    VkRenderingAttachmentInfo depthAttachment{};
    depthAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.imageView   = m_Context.depthImageView;
    
    depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    depthAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.clearValue  = clearValues[1];


    VkRenderingInfo renderingInfo{};
    renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset    = {0, 0};
    renderingInfo.renderArea.extent    = m_Context.swapChainExtent;
    renderingInfo.layerCount           = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments    = &colorAttachment;
    renderingInfo.pDepthAttachment     = &depthAttachment;

    

    vkCmdBeginRendering(cmd, &renderingInfo);

    // Bind del pipeline gráfico
    vkCmdBindPipeline(m_Context.commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Context.Pipeline);

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
    viewport.width    = static_cast<float>(m_Context.swapChainExtent.width);
    viewport.height   = static_cast<float>(m_Context.swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    // Scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_Context.swapChainExtent;

    vkCmdSetViewport(m_Context.commandBuffers[currentFrame], 0, 1, &viewport); //??
    vkCmdSetScissor(cmd, 0, 1, &scissor);


    VkBuffer vertexBuffers[] = { m_Context.vertexBuffer };
    VkDeviceSize offsets[]   = { 0 };
    
    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmd, m_Context.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(cmd, 
                        VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        m_Context.pipelineLayout, 
                        0, 1, 
                        &m_Context.descriptorSets[currentFrame],   // Asumiendo que es un método de Texture
                        0, nullptr);

    vkCmdDrawIndexed(cmd, static_cast<uint32_t>(mesh.getIndices().size()), 1, 0, 0, 0);

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

    vkCmdEndRendering(cmd);


    transition_image_layout(
        cmd,
        m_Context.swapchainImages[imageIndex],
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

