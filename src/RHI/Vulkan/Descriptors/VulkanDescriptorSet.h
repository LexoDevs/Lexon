#include "../Core/VulkanContext.h"
#include <iostream>
#include "../../../Assets/Loaders/LoaderAssets.h"


class VulkanDescriptorSet {
    public:
        VulkanDescriptorSet();
        ~VulkanDescriptorSet();

        void createDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, VkBuffer uniformBuffers[]);
        void destroyDescriptorSet();
        void bindDescriptorSet(uint32_t currentFrame,VkCommandBuffer commandBuffers[], VkPipelineLayout pipelineLayout);
void DestroyDescriptorSetLayout();
        void CreateDescriptorSetLayout(VkDevice device);
        VkDescriptorSetLayout GetDescriptorSetLayout(){return descriptorSetLayout;};
        VkDescriptorSet GetDescriptorSet(int frame){return descriptorSets[frame];};

    private:
    VkDescriptorSet descriptorSets[MAX_FRAMES_IN_FLIGHT];
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkDescriptorSetLayout descriptorSetLayout;
VkDevice cp_device;
};