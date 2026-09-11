#include "VulkanDescriptorSet.h"

VulkanDescriptorSet::VulkanDescriptorSet(){};


VulkanDescriptorSet::~VulkanDescriptorSet(){
    destroyDescriptorSet();
    DestroyDescriptorSetLayout();
};

void VulkanDescriptorSet::DestroyDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(cp_device, descriptorSetLayout, nullptr);

}

void VulkanDescriptorSet::bindDescriptorSet(
    uint32_t frame,
    uint32_t materialIndex,
    VkCommandBuffer commandBuffer,
    VkPipelineLayout pipelineLayout
){
    const size_t descriptorIndex = GetDescriptorIndex(frame, materialIndex);
    const VkDescriptorSet selectedDescriptorSet = descriptorSets[descriptorIndex];
    
    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipelineLayout,
        0,
        1,
        &selectedDescriptorSet,
        0,
        nullptr
    );};

void VulkanDescriptorSet::createDescriptorSets(
    VkDevice device,
    VkDescriptorPool descriptorPool,
    VkBuffer uniformBuffers[],
    const std::vector<VkImageView>& materialImageViews,
    VkSampler textureSampler) {
    
        cp_device = device;

        materialCount = static_cast<uint32_t>(materialImageViews.size());
        const uint32_t descriptorCount = materialCount * MAX_FRAMES_IN_FLIGHT;

        descriptorSets.resize(descriptorCount);
std::cout<<"la puta"<<std::endl;


        std::vector<VkDescriptorSetLayout> layouts(descriptorCount, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = descriptorCount;
        allocInfo.pSetLayouts = layouts.data();

        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
std::cout<<"la puta"<<std::endl;

    for (uint32_t materialIndex = 0; materialIndex < materialCount; ++materialIndex) {
        
        for (uint32_t frame = 0; frame < MAX_FRAMES_IN_FLIGHT; ++frame){
    
            const size_t descriptorIndex = GetDescriptorIndex(frame, materialIndex);

            // === 1. Uniform Buffer (binding 0) ===
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[frame];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            // === 2. Combined Image Sampler (binding 1) ===
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = materialImageViews[materialIndex]; //corregir
            imageInfo.sampler = textureSampler;

            VkWriteDescriptorSet uboWrite{};
            uboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            uboWrite.dstSet = descriptorSets[descriptorIndex];
            uboWrite.dstBinding = 0;
            //uboWrite.dstArrayElement = 0;
            uboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            uboWrite.descriptorCount = 1;
            uboWrite.pBufferInfo = &bufferInfo;


std::cout<<"la puta"<<std::endl;

            VkWriteDescriptorSet textureWrite{};
            textureWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            textureWrite.dstSet = descriptorSets[descriptorIndex];
            textureWrite.dstBinding = 1;                    // ← importante
            //textureWrite.dstArrayElement = 0;
            textureWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            textureWrite.descriptorCount = 1;
            textureWrite.pImageInfo = &imageInfo;

            //Actualizar ambos de una vez
            std::array<VkWriteDescriptorSet, 2> writes = {uboWrite, textureWrite};

            vkUpdateDescriptorSets(device,2,writes.data(),0, nullptr);
        }
    }
    std::cout<<"DescriptorSetCreado"<<std::endl;
}


void VulkanDescriptorSet::destroyDescriptorSet(){

    descriptorSets.clear();
    materialCount = 0;
};


void VulkanDescriptorSet::CreateDescriptorSetLayout(VkDevice device)
{
    cp_device = device;

    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags =
        VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.stageFlags =
        VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    const std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        uboLayoutBinding,
        samplerLayoutBinding
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount =
        static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(
            device,
            &layoutInfo,
            nullptr,
            &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "failed to create descriptor set layout!"
        );
    }
}

size_t VulkanDescriptorSet::GetDescriptorIndex(
    uint32_t frame,
    uint32_t materialIndex) const
{
    return static_cast<size_t>(materialIndex)
        * MAX_FRAMES_IN_FLIGHT
        + frame;
}