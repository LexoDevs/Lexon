
#include "VulkanVertexBuffer.h"


VertexBuffer::~VertexBuffer(){
    destroyVertexBuffer();
};


void VertexBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
     VkMemoryPropertyFlags properties, VkDevice device,
      VkPhysicalDevice physicalDevice,VkBuffer& buffer, 
    VkDeviceMemory& bufferMemory){

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}


void VertexBuffer::createVertexBuffer(CpuModel& mesh, VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer){
    cp_device = device;
    cp_commandPool = commandPool;

    std::vector<CpuVertex> allVertices;
    size_t totalVertices = 0;

    for (const auto& submesh : mesh.meshes)
    {
        totalVertices += submesh.vertices.size();
    }

    allVertices.reserve(totalVertices);

    meshRanges.clear();
    meshRanges.reserve(mesh.meshes.size());

    uint32_t firstVertex = 0;

    for (const auto& submesh : mesh.meshes)
    {
        GPUMeshRange range{};

        range.firstVertex = firstVertex;
        range.vertexCount =
            static_cast<uint32_t>(submesh.vertices.size());

        meshRanges.push_back(range);

        allVertices.insert(
            allVertices.end(),
            submesh.vertices.begin(),
            submesh.vertices.end()
        );

        firstVertex += range.vertexCount;
    }



    VkDeviceSize bufferSize = sizeof(CpuVertex) * allVertices.size();
    std::cout<<"Numero de vertices: "<<totalVertices<<std::endl;
    std::cout<<"Tamano de buffer: "<<std::setprecision(3)<<(float)bufferSize/1000000<<" Mb"<<std::endl;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,device,physicalDevice,stagingBuffer, stagingBufferMemory);


    void* data;
    std::cout<<device<<std::endl;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, allVertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,device,physicalDevice,vertexBuffer,vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize,graphicsQueue, commandBuffer);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VertexBuffer::destroyVertexBuffer(){

        vkDestroyBuffer(cp_device, vertexBuffer, nullptr);
        vkFreeMemory(cp_device, vertexBufferMemory, nullptr);
        
}




uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                VulkanMemoryTypeIndex = i;
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
}




void VertexBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,VkQueue graphicsQueue,VkCommandBuffer commandBuffer) {

    VkCommandBuffer newcommandBuffer = beginSingleTimeCommands(cp_commandPool, cp_device, commandBuffer);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(newcommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(cp_commandPool, cp_device, newcommandBuffer, graphicsQueue);

}

VkCommandBuffer VertexBuffer::beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device,VkCommandBuffer commandBuffer) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VertexBuffer::endSingleTimeCommands(VkCommandPool commandPool, VkDevice device,VkCommandBuffer commandBuffer,VkQueue graphicsQueue) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}











