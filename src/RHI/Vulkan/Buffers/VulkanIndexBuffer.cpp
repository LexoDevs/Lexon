#include "VulkanIndexBuffer.h"

IndexBuffer::~IndexBuffer(){

    destroyBuffer();
};


void IndexBuffer::createIndexBuffer(CpuModel& mesh, VkDevice device, VkPhysicalDevice physicalDevice,VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer){

    cp_device = device;
    cp_commandPool = commandPool;
    std::vector<uint32_t> allindex;
    size_t totalindex = 0;

uint32_t firstIndex = 0;
uint32_t firstVertex = 0;

for (const auto& submesh : mesh.meshes)
    {
        totalindex += submesh.indices.size();
    }


    allindex.reserve(totalindex);



    meshRanges.reserve(mesh.meshes.size());

    uint32_t firstindex = 0;

    for (const auto& submesh : mesh.meshes)
    {
        GPUMeshRange range{};

        range.firstIndex = firstindex;
        range.indexCount =
            static_cast<uint32_t>(submesh.indices.size());

        range.firstVertex = firstVertex;
        range.vertexCount =
        static_cast<uint32_t>(submesh.vertices.size());

        meshRanges.push_back(range);

        allindex.insert(
            allindex.end(),
            submesh.indices.begin(),
            submesh.indices.end()
        );

        firstindex += range.indexCount;
            firstVertex += range.vertexCount;

    }

    VkDeviceSize bufferSize = sizeof(uint32_t) * allindex.size();
    std::cout<<"Numero de indices: "<<totalindex<<std::endl;
    std::cout<<"Tamano de buffer: "<<std::setprecision(3)<<(float)bufferSize/1000000<<" Mb"<<std::endl;


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

void IndexBuffer::destroyBuffer(){

    std::cout<<"Index Buffer destruido"<<std::endl;
    vkDestroyBuffer(cp_device, indexBuffer, nullptr);
    vkFreeMemory(cp_device, indexBufferMemory, nullptr);

}