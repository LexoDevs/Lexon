#include "VulkanCommandPool.h"

void VulkanCommandPool::createCommandPool() {

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    auto queueFamilyIndextemp = m_Context.QueueFamilie.graphicsFamily.value();
    poolInfo.queueFamilyIndex = queueFamilyIndextemp;

    if (vkCreateCommandPool(m_Context.device, &poolInfo, nullptr, &m_Context.commandPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
    }
    std::cout<<"comand pool creado"<<std::endl;

}

void VulkanCommandPool::destroyCommandPool() {

    vkDestroyCommandPool(m_Context.device, m_Context.commandPool, nullptr);

}