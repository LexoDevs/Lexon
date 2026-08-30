#include "VulkanCommandBuffer.h"

    CommandBuffer::CommandBuffer(){

    };

void CommandBuffer::createCommandBuffer(VkDevice device, VkCommandPool commandPool){

    std::cout << "\033[1;36m[!] Creando Buffer de Comandos...\033[0m\n";

    VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}



