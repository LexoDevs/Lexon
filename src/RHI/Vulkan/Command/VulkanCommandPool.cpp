#include "VulkanCommandPool.h"

VulkanCommandPool::VulkanCommandPool(){

};

VulkanCommandPool::~VulkanCommandPool(){
    destroyCommandPool();
};


void VulkanCommandPool::createCommandPool(VkDevice device, QueueFamilyIndices QueueFamilie) {
    
    cp_device = device;

    std::cout << "\033[1;36m[!] Creando cadena pool de instrucciones...\033[0m\n";


    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (!QueueFamilie.graphicsFamily.has_value())
    {
        throw std::runtime_error(
            "VulkanCommandPool: graphicsFamily no tiene valor"
        );
    }

    auto queueFamilyIndextemp = QueueFamilie.graphicsFamily.value();
    poolInfo.queueFamilyIndex = queueFamilyIndextemp;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
    }
    std::cout << "\t\033[1;32m CommandPool\033[0m creada correctamente en \033[1;32m" << commandPool << "\033[0m\n";

}

void VulkanCommandPool::destroyCommandPool() {

    std::cout<<"Se ha destruido el command pool"<<std::endl;
    vkDestroyCommandPool(cp_device, commandPool, nullptr);

}