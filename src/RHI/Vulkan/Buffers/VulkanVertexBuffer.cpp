
#include "VulkanVertexBuffer.h"





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





















