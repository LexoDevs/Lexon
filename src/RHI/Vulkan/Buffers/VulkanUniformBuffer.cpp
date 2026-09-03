#include "VulkanUniformBuffer.h"
#include <chrono>


UniformBuffer::UniformBuffer(){};
UniformBuffer::~UniformBuffer(){

    destroyBuffer();
};


void UniformBuffer::createUniformBuffer(VkDevice device, VkPhysicalDevice physicaldevice){
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    cp_device = device;
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device, physicaldevice, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
};

void UniformBuffer::destroyBuffer(){

    std::cout<<"Uniform Buffer destruido"<<std::endl;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

        vkDestroyBuffer(cp_device, uniformBuffers[i], nullptr);
        vkFreeMemory(cp_device, uniformBuffersMemory[i], nullptr);
    }

}

void UniformBuffer::updateUniformBuffer(uint32_t currentImage, std::vector<CpuMesh> mesh, CameraView camera,VkExtent2D swapChainExtent) {
    
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    float aspectratio = swapChainExtent.width / (float) swapChainExtent.height;

ObjectInstance model{};

    model.SetMatrixModel(UBO);
    camera.SetCameraView(UBO, aspectratio);

    memcpy(uniformBuffersMapped[currentImage], &UBO, sizeof(UBO));

}

