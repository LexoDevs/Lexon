#include "VulkanIndexBuffer.h"

        IndexBuffer::~IndexBuffer(){

        };


void IndexBuffer::createIndexBuffer(CpuModel& mesh, VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer){

    cp_device = device;

    std::vector<uint32_t> allindex;
    size_t totalindex = 0;

for (const auto& submesh : mesh.meshes)
    {
        totalindex += submesh.indices.size();
    }


    allindex.reserve(totalindex);

meshRanges.clear();

    meshRanges.reserve(mesh.meshes.size());

    uint32_t firstindex = 0;

    for (const auto& submesh : mesh.meshes)
    {
        GPUMeshRange range{};

        range.firstIndex = firstindex;
        range.indexCount =
            static_cast<uint32_t>(submesh.indices.size());

        meshRanges.push_back(range);

        allindex.insert(
            allindex.end(),
            submesh.indices.begin(),
            submesh.indices.end()
        );

        firstindex += range.indexCount;
    }

    VkDeviceSize bufferSize = sizeof(uint32_t) * allindex.size();


    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device, physicalDevice, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, allindex.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(device, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device, physicalDevice, indexBuffer, indexBufferMemory);
    std::cout<<"error"<<std::endl;

    copyBuffer(stagingBuffer, indexBuffer, bufferSize,graphicsQueue, commandBuffer);
    std::cout<<"error"<<std::endl;

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
    std::cout<<"error"<<std::endl;

}

void IndexBuffer::destroyIndexBuffer(){
    
        vkDestroyBuffer(cp_device, indexBuffer, nullptr);
    vkFreeMemory(cp_device, indexBufferMemory, nullptr);
}