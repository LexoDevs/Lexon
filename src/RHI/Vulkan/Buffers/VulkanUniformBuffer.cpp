#include "VulkanUniformBuffer.h"
#include <chrono>

void UniformBuffer::createUniformBuffer(){
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    m_Context.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    m_Context.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createeBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Context.uniformBuffers[i], m_Context.uniformBuffersMemory[i]);

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


    
    mesh.SetMatrixModel(m_Context.UBO, time);
    camera.SetCameraView(m_Context.UBO, aspectratio, time);

    memcpy(m_Context.uniformBuffersMapped[currentImage], &m_Context.UBO, sizeof(m_Context.UBO));

}

