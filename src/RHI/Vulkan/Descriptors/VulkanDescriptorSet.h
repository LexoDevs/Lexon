#include "../Core/VulkanContext.h"
#include <iostream>
#include "../../../Assets/Loaders/LoaderAssets.h"


class VulkanDescriptorSet {
    public:
        VulkanDescriptorSet();
        ~VulkanDescriptorSet();

        void createDescriptorSets(
    VkDevice device,
    VkDescriptorPool descriptorPool,
    VkBuffer uniformBuffers[],
    const std::vector<VkImageView>& materialImageViews,
    VkSampler textureSampler);

size_t GetDescriptorIndex( uint32_t frame, uint32_t materialIndex) const;


        void destroyDescriptorSet();
        
void bindDescriptorSet(
    uint32_t frame,
    uint32_t materialIndex,
    VkCommandBuffer commandBuffer,
    VkPipelineLayout pipelineLayout
);

void DestroyDescriptorSetLayout();
        void CreateDescriptorSetLayout(VkDevice device);
        VkDescriptorSetLayout GetDescriptorSetLayout(){return descriptorSetLayout;};
        VkDescriptorSet GetDescriptorSet(int frame){return descriptorSets[frame];};
    uint32_t GetMaterialCount(){return materialCount;};




    private:
    std::vector<VkDescriptorSet> descriptorSets;
    uint32_t materialCount = 0;

    VkDescriptorSetLayout descriptorSetLayout;
VkDevice cp_device;
};