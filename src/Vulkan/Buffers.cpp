#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Buffers.h"



void Pool::createCommandPool(LogicalDevice logicaldevice, PhysicalDevice physicaldevice) {

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    auto queueFamilyIndextemp = logicaldevice.GetQueueFamilies().graphicsFamily.value();
    poolInfo.queueFamilyIndex = queueFamilyIndextemp;

    if (vkCreateCommandPool(logicaldevice.GetLogicalDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
    }
    std::cout<<"comand pool creado"<<std::endl;

}

void Pool::destroyCommandPool(LogicalDevice logicaldevice) {

    vkDestroyCommandPool(logicaldevice.GetLogicalDevice(), commandPool, nullptr);
    vkDestroyDescriptorPool(logicaldevice.GetLogicalDevice(), descriptorPool, nullptr);

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

void DepthBuffer::recordCommandBuffer(uint32_t imageIndex, Swapchain swapchain, GraphicsPipeline pipeline, uint32_t currentFrame, VertexBuffer& vertexbuffer, Texture texture, VkClearValue clearDepth, VkImage depthImage)
{
    VkCommandBuffer cmd = vertexbuffer.getCommandBuffer(currentFrame);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    

    transition_image_layout(
        cmd,
        swapchain.getSwapchainImages()[imageIndex],
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
        getdepthImage(),
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
    colorAttachment.imageView   = swapchain.getSwapchainImageView()[imageIndex];  // Asume que tienes este método
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue  = clearValues[0];



    VkRenderingAttachmentInfo depthAttachment{};
    depthAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.imageView   = getdepthImageView();
    depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    depthAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.clearValue  = clearValues[1];


    VkRenderingInfo renderingInfo{};
    renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset    = {0, 0};
    renderingInfo.renderArea.extent.width    = swapchain.getExtentSwapchain().width;
    renderingInfo.renderArea.extent.height    = swapchain.getExtentSwapchain().height;
    renderingInfo.layerCount           = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments    = &colorAttachment;
    renderingInfo.pDepthAttachment     = &depthAttachment;

    
std::cout << "RenderArea: " << renderingInfo.renderArea.extent.width << "x" << renderingInfo.renderArea.extent.height << "\n";
std::cout << "Depth View size: " << getDepthSize().width << "x" << getDepthSize().height << "\n";


    vkCmdBeginRendering(cmd, &renderingInfo);

    // Bind del pipeline gráfico
    vkCmdBindPipeline(vertexbuffer.getCommandBuffer(currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGrapicsPipeline());

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
    viewport.width    = static_cast<float>(swapchain.getExtentSwapchain().width);
    viewport.height   = static_cast<float>(swapchain.getExtentSwapchain().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    // Scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain.getExtentSwapchain();

    vkCmdSetViewport(vertexbuffer.getCommandBuffer(currentFrame), 0, 1, &viewport); //??
    vkCmdSetScissor(cmd, 0, 1, &scissor);


    VkBuffer vertexBuffers[] = { vertexbuffer.getVertexBuffer() };
    VkDeviceSize offsets[]   = { 0 };
    
    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmd, vertexbuffer.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(cmd, 
                        VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        pipeline.getGrapicsPipelineLayout(), 
                        0, 1, 
                        &texture.getdescriptorSets(currentFrame),   // Asumiendo que es un método de Texture
                        0, nullptr);

    vkCmdDrawIndexed(cmd, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRendering(cmd);


    transition_image_layout(
        cmd,
        swapchain.getSwapchainImages()[imageIndex],
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



void copyBuffer(LogicalDevice logicaldevice, Pool commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, Texture texture) {

    VkCommandBuffer commandBuffer = texture.beginSingleTimeCommands(commandPool, logicaldevice);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    texture.endSingleTimeCommands(commandBuffer, logicaldevice,commandPool);

    }

void VertexBuffer::destroyVertexBuffer(LogicalDevice logicaldevice){

        vkDestroyBuffer(logicaldevice.GetLogicalDevice(), indexBuffer, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), indexBufferMemory, nullptr);
        vkDestroyBuffer(logicaldevice.GetLogicalDevice(), vertexBuffer, nullptr);
        vkFreeMemory(logicaldevice.GetLogicalDevice(), vertexBufferMemory, nullptr);
        
}

void VertexBuffer::createVertexBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool commandPool, VkImage texture){

    Texture textclass{};
    textclass.getTextureImage() = texture;

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory);

    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    copyBuffer(logicaldevice, commandPool,stagingBuffer, vertexBuffer, bufferSize,textclass);

            vkDestroyBuffer(logicaldevice.GetLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, nullptr);

}

void VertexBuffer::createIndexBuffer(LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool commandPool, VkImage texture){

    Texture textclass{};
    textclass.getTextureImage() = texture;

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory);

    createBuffer(logicaldevice,physicaldevice,bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(logicaldevice,commandPool,stagingBuffer, indexBuffer, bufferSize, textclass);

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

void Pool::createDescriptorPool(LogicalDevice logicaldevice)
{
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(logicaldevice.GetLogicalDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}



    


void Texture::createTextureImage(VertexBuffer buffer, LogicalDevice logicaldevice, PhysicalDevice physicaldevice, Pool pool){
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("../resources/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    std::cout<<"Imagen cargada en memoria correctamente"<<std::endl;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    buffer.createBuffer(logicaldevice, physicaldevice,imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory);

    stbi_image_free(pixels);

    createImage(logicaldevice,buffer,  physicaldevice, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

    transitionImageLayout(pool,logicaldevice,textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    copyBufferToImage(pool,logicaldevice,stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
    transitionImageLayout(pool, logicaldevice,textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(logicaldevice.GetLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), stagingBufferMemory, nullptr);
}


void Texture::createImage(LogicalDevice logicaldevice, VertexBuffer buffer, PhysicalDevice physicaldevice, uint32_t width, uint32_t height, VkFormat format,
    VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(logicaldevice.GetLogicalDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(logicaldevice.GetLogicalDevice(), image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = buffer.findMemoryType(memRequirements.memoryTypeBits, properties, physicaldevice);

        if (vkAllocateMemory(logicaldevice.GetLogicalDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(logicaldevice.GetLogicalDevice(), image, imageMemory, 0);
}


void Texture::transitionImageLayout(Pool pool, LogicalDevice logicaldevice, VkImage &image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands(pool, logicaldevice);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

        endSingleTimeCommands(commandBuffer, logicaldevice, pool);
}

VkCommandBuffer Texture::beginSingleTimeCommands(Pool pool, LogicalDevice logicaldevice) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = pool.getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicaldevice.GetLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Texture::endSingleTimeCommands(VkCommandBuffer commandBuffer, LogicalDevice logicaldevice, Pool pool) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(logicaldevice.getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicaldevice.getGraphicQueue());

    vkFreeCommandBuffers(logicaldevice.GetLogicalDevice(), pool.getCommandPool(), 1, &commandBuffer);
}
    
void Texture::copyBufferToImage(Pool pool, LogicalDevice logicaldevice, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands(pool, logicaldevice);

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        endSingleTimeCommands(commandBuffer, logicaldevice, pool);
}

void Texture::destroyDescriptorSet(LogicalDevice logicaldevice){

    for (int i = 0; i<MAX_FRAMES_IN_FLIGHT;i++){
    descriptorSets[i] = VK_NULL_HANDLE;

    }
};


void Texture::destroyImageTexture(LogicalDevice logicaldevice){
    vkDestroyImage(logicaldevice.GetLogicalDevice(), textureImage, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), textureImageMemory, nullptr);


};

void Texture::createTextureImageView(LogicalDevice logicaldevice, VkImageAspectFlags aspectflags){
        textureImageView = createImageView(logicaldevice, textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

};

void Texture::destroyImageTextureView(LogicalDevice logicaldevice){
    vkDestroySampler(logicaldevice.GetLogicalDevice(), textureSampler, nullptr);

    vkDestroyImageView(logicaldevice.GetLogicalDevice(), textureImageView, nullptr);

};




VkImageView Texture::createImageView(LogicalDevice logicaldevice, VkImage image, VkFormat format,  VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(logicaldevice.GetLogicalDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image view!");
        }

        return imageView;
    }

    void Texture::createImageViews(Swapchain swapchain, LogicalDevice logicaldevice, VkImageAspectFlags aspectflags){
        swapchain.getSwapchainImageView().resize(swapchain.getSwapchainImages().size());

    for (uint32_t i = 0; i < swapchain.getSwapchainImages().size(); i++) {
         swapchain.getSwapchainImageView()[i] = createImageView(logicaldevice, swapchain.getSwapchainImages()[i], swapchain.GetswapChainImageformat(), aspectflags);
    }
};


void Texture::createTextureSampler(LogicalDevice logicaldevice, PhysicalDevice physicaldevice){
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicaldevice.GetPhysicalDevice(), &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        if (vkCreateSampler(logicaldevice.GetLogicalDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }

};

void Texture::bindDescriptorSet(VertexBuffer vertexbuffer, GraphicsPipeline pipeline,uint32_t currentFrame){
    vkCmdBindDescriptorSets(vertexbuffer.getCommandBuffer(currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGrapicsPipelineLayout(), 0, 1, &getdescriptorSets(currentFrame), 0, nullptr);
};

void Texture::createDescriptorSets(LogicalDevice logicaldevice, GraphicsPipeline pipeline, Pool pool,  VertexBuffer vertexbuffer) {
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
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                
    // === 1. Uniform Buffer (binding 0) ===
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = vertexbuffer.getUniformBuffer(i);
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet uboWrite{};
    uboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    uboWrite.dstSet = descriptorSets[i];
    uboWrite.dstBinding = 0;
    uboWrite.dstArrayElement = 0;
    uboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboWrite.descriptorCount = 1;
    uboWrite.pBufferInfo = &bufferInfo;

    // === 2. Combined Image Sampler (binding 1) ===
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = textureImageView;
    imageInfo.sampler = textureSampler;

    VkWriteDescriptorSet samplerWrite{};
    samplerWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    samplerWrite.dstSet = descriptorSets[i];
    samplerWrite.dstBinding = 1;                    // ← importante
    samplerWrite.dstArrayElement = 0;
    samplerWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerWrite.descriptorCount = 1;
    samplerWrite.pImageInfo = &imageInfo;

    // Actualizar ambos de una vez
    std::array<VkWriteDescriptorSet, 2> descriptorWrites = {uboWrite, samplerWrite};

    vkUpdateDescriptorSets(logicaldevice.GetLogicalDevice(),
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(),
                           0, nullptr);
        }
}
}


void DepthBuffer::createDepthResources(PhysicalDevice physicaldevice, Texture texture, LogicalDevice logicaldevice, VertexBuffer buffer, Swapchain& swapchain){
        
        VkFormat depthFormat = findDepthFormat(physicaldevice);
        
        getDepthSize().height = swapchain.getExtentSwapchain().height;
        getDepthSize().width = swapchain.getExtentSwapchain().width;

        texture.createImage(logicaldevice, buffer, physicaldevice, getDepthSize().width, getDepthSize().height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, getdepthImage(), depthImageMemory);
        depthImageView = texture.createImageView(logicaldevice, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT );
        
};


VkFormat DepthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, PhysicalDevice physicaldevice) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicaldevice.GetPhysicalDevice(), format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }


VkFormat DepthBuffer::findDepthFormat(PhysicalDevice physicaldevice) {
    return findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,physicaldevice
    );
}


bool DepthBuffer::hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

void DepthBuffer::destroyDepthResources(LogicalDevice logicaldevice){

    vkDestroyImageView(logicaldevice.GetLogicalDevice(), depthImageView, nullptr);
    vkDestroyImage(logicaldevice.GetLogicalDevice(), depthImage, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), depthImageMemory, nullptr);
};

void DepthBuffer::cleanDepthResources(LogicalDevice logicaldevice){
    vkDestroyImageView(logicaldevice.GetLogicalDevice(), depthImageView, nullptr);
    vkDestroyImage(logicaldevice.GetLogicalDevice(), depthImage, nullptr);
    vkFreeMemory(logicaldevice.GetLogicalDevice(), depthImageMemory, nullptr);

};
