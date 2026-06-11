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

void VertexBuffer::createCommandBuffer(LogicalDevice logicaldevice, Pool pool){

    VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = pool.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    if (vkAllocateCommandBuffers(logicaldevice.GetLogicalDevice(), &allocInfo, commandBuffers) != VK_SUCCESS) {
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
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2; 
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
            dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		    dependency_info.dependencyFlags         = {};
		    dependency_info.imageMemoryBarrierCount = 1;
		    dependency_info.pImageMemoryBarriers    = &barrier;

    vkCmdPipelineBarrier2(commandBuffer, &dependency_info);
}

void VertexBuffer::recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline, uint32_t currentFrame, VertexBuffer& vertexbuffer)
{


    VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


        if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
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
        commandBuffers[currentFrame]
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

    vkCmdBeginRendering(commandBuffers[currentFrame], &renderingInfo);

    // Bind del pipeline gráfico
    vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGrapicsPipeline());



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

    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

    VkBuffer vertexBuffers[] = {vertexbuffer.getVertexBuffer()};
    VkDeviceSize offsets[] = {0};
    
    vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffers[currentFrame], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGrapicsPipelineLayout(), 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    // Dibujar el triángulo
    vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    // ====================== FIN DEL RENDERING ======================
    vkCmdEndRendering(commandBuffers[currentFrame]);

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
        commandBuffers[currentFrame]
    );

    if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}


uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,PhysicalDevice physicaldevice) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicaldevice.GetPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
}

void VertexBuffer::createBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice,VkDeviceSize size,
    VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
    VkBuffer& buffer, VkDeviceMemory& bufferMemory){

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(logicaldevice.GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicaldevice.GetLogicalDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties,physicaldevice);

    if (vkAllocateMemory(logicaldevice.GetLogicalDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(logicaldevice.GetLogicalDevice(), buffer, bufferMemory, 0);
}



void copyBuffer(LogicalDevice logicaldevice, Pool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool.getCommandPool();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(logicaldevice.GetLogicalDevice(), &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

            VkBufferCopy copyRegion{};
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(logicaldevice.getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(logicaldevice.getGraphicQueue());

        vkFreeCommandBuffers(logicaldevice.GetLogicalDevice(), commandPool.getCommandPool(), 1, &commandBuffer);
    }

void VertexBuffer::destroyVertexBuffer(LogicalDevice logicaldevice){

        vkDestroyBuffer(logicaldevice.GetLogicalDevice(), indexBuffer, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), indexBufferMemory, nullptr);
        vkDestroyBuffer(logicaldevice.GetLogicalDevice(), vertexBuffer, nullptr);
        vkFreeMemory(logicaldevice.GetLogicalDevice(), vertexBufferMemory, nullptr);
        
}

void VertexBuffer::createVertexBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool commandPool){

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory);

    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    copyBuffer(logicaldevice, commandPool,stagingBuffer, vertexBuffer, bufferSize);

            vkDestroyBuffer(logicaldevice.GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, nullptr);

}

void VertexBuffer::createIndexBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool commandPool){

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory);

    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(logicaldevice,commandPool,stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(logicaldevice.GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, nullptr);

}

void VertexBuffer::createUniformBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice){
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(logicaldevice.GetLogicalDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }

};

void VertexBuffer::destroyUniformBuffer(LogicalDevice logicaldevice){
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(logicaldevice.GetLogicalDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(logicaldevice.GetLogicalDevice(), uniformBuffersMemory[i], nullptr);
    }

}

void VertexBuffer::updateUniformBuffer(Swapchain swapchain, uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapchain.getExtentSwapchain().width / (float) swapchain.getExtentSwapchain().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Pool::createDescriptorPool(LogicalDevice logicaldevice){

        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(logicaldevice.GetLogicalDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

};

    void VertexBuffer::createDescriptorSets(LogicalDevice logicaldevice, GraphicsPipeline pipeline, Pool pool) {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, pipeline.getdescriptorSetLayout());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool.getDescriptorPool();
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        if (vkAllocateDescriptorSets(logicaldevice.GetLogicalDevice(), &allocInfo, descriptorSets) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = getUniformBuffer(i);
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(logicaldevice.GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
        }
    }