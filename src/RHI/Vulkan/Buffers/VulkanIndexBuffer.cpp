#include "VulkanIndexBuffer.h"


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