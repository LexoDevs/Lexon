#include "VulkanDescriptorSet.h"

VulkanDescriptorSet::VulkanDescriptorSet(){};


VulkanDescriptorSet::~VulkanDescriptorSet(){
    destroyDescriptorSet();
    DestroyDescriptorSetLayout();
};

void VulkanDescriptorSet::DestroyDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(cp_device, descriptorSetLayout, nullptr);

}

void VulkanDescriptorSet::bindDescriptorSet(uint32_t currentFrame, VkCommandBuffer commandBuffers[], VkPipelineLayout pipelineLayout){

    vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
};

void VulkanDescriptorSet::createDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, VkBuffer uniformBuffers[],
    std::vector<VkImageView> textureImageView, VkSampler textureSampler) {
    
        cp_device = device;


        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT*TEXTURE_PATHS.size(); i++) {

    // === 1. Uniform Buffer (binding 0) ===
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i];
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
    imageInfo.imageView = textureImageView[i]; //corregir
    imageInfo.sampler = textureSampler;

    VkWriteDescriptorSet samplerWrite{};
    samplerWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    samplerWrite.dstSet = descriptorSets[i];
    samplerWrite.dstBinding = 1;                    // ← importante
    samplerWrite.dstArrayElement = 0;
    samplerWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerWrite.descriptorCount = 1;
    samplerWrite.pImageInfo = &imageInfo;

     //Actualizar ambos de una vez
    std::array<VkWriteDescriptorSet, 2> descriptorWrites = {uboWrite, samplerWrite};
    //VkWriteDescriptorSet descriptorWrite= uboWrite;

    vkUpdateDescriptorSets(device,
                           2,
                           descriptorWrites.data(),
                           0, nullptr);
        }
}


void VulkanDescriptorSet::destroyDescriptorSet(){

    for (int i = 0; i<MAX_FRAMES_IN_FLIGHT;i++){
    descriptorSets[i] = VK_NULL_HANDLE;

    }
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

