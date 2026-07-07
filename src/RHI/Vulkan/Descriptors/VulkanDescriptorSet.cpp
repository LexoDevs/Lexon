#include "VulkanDescriptorSet.h"


void VulkanDescriptorSet::bindDescriptorSet(uint32_t currentFrame){
    vkCmdBindDescriptorSets(m_Context.commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Context.pipelineLayout, 0, 1, &m_Context.descriptorSets[currentFrame], 0, nullptr);
};

void VulkanDescriptorSet::createDescriptorSets() {
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_Context.descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_Context.descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        if (vkAllocateDescriptorSets(m_Context.device, &allocInfo, m_Context.descriptorSets) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                
    // === 1. Uniform Buffer (binding 0) ===
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = m_Context.uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet uboWrite{};
    uboWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    uboWrite.dstSet = m_Context.descriptorSets[i];
    uboWrite.dstBinding = 0;
    uboWrite.dstArrayElement = 0;
    uboWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboWrite.descriptorCount = 1;
    uboWrite.pBufferInfo = &bufferInfo;

    // === 2. Combined Image Sampler (binding 1) ===
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = m_Context.textureImageView;
    imageInfo.sampler = m_Context.textureSampler;

    VkWriteDescriptorSet samplerWrite{};
    samplerWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    samplerWrite.dstSet = m_Context.descriptorSets[i];
    samplerWrite.dstBinding = 1;                    // ← importante
    samplerWrite.dstArrayElement = 0;
    samplerWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerWrite.descriptorCount = 1;
    samplerWrite.pImageInfo = &imageInfo;

    // Actualizar ambos de una vez
    std::array<VkWriteDescriptorSet, 2> descriptorWrites = {uboWrite, samplerWrite};

    vkUpdateDescriptorSets(m_Context.device,
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(),
                           0, nullptr);
        }
}
}

void VulkanDescriptorSet::destroyDescriptorSet(){

    for (int i = 0; i<MAX_FRAMES_IN_FLIGHT;i++){
    m_Context.descriptorSets[i] = VK_NULL_HANDLE;

    }
};
