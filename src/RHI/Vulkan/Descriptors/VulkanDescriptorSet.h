#include "../Core/VulkanContext.h"
#include <iostream>
#include "../../../Assets/Loaders/LoaderAssets.h"


class VulkanDescriptorSet {
    public:
        VulkanDescriptorSet(VulkanContext& context)
        : m_Context(context)              
    {
    }
        void createDescriptorSets();
        void destroyDescriptorSet();
        void bindDescriptorSet(uint32_t currentFrame);


    private:
    VulkanContext& m_Context;

};