#include "VulkanDescriptorPool.h"

VulkanDescriptorPool::~VulkanDescriptorPool(){
    if (initialiced == false){
    return;
    }

    destroyDescriptorPool();
    initialiced = false ;
};

void VulkanDescriptorPool::createDescriptorPool(VkDevice device)
{

    std::cout << "\033[1;36m[!] Creando Descriptor Pool...\033[0m\n";

    cp_device = device;

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(cp_device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
    initialiced = true;
}

void VulkanDescriptorPool::destroyDescriptorPool() {


    vkDestroyDescriptorPool(cp_device, descriptorPool, nullptr);


}

void VulkanDescriptorPool::CreateImGuiDescriptorPool(VkDevice device)
{
    std::cout << "\033[1;36m[!] Creando Descriptor Pool del HUD...\033[0m\n";

    cp_device = device;
    const uint32_t maxSets = 1000;   // Suficiente para docking + viewports

    std::cout<<"Error antes"<<std::endl;
    std::vector<VkDescriptorPoolSize> poolSizes = {
        { VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
        { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         500  }
    };

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT; // Muy importante para ImGui
    poolInfo.maxSets       = maxSets;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes    = poolSizes.data();

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create imgui descriptor pool!");
    }

    std::cout<<"Error despues"<<std::endl;
    initialiced = true;

}