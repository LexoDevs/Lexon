#include "../Core/VulkanContext.h"
#include "../Buffers/VulkanVertexBuffer.h"
#include "../Helpers/VulkanConstants.h"
#include <iostream>
    
class CommandBuffer{
    public:

    CommandBuffer();
    ~CommandBuffer();

    void createCommandBuffer(VkDevice device, VkCommandPool commandPool);
    VkCommandBuffer GetCommandBuffer(uint32_t frame){return commandBuffer[frame];};
    
    private:
    VkCommandBuffer commandBuffer[MAX_FRAMES_IN_FLIGHT]{};
VkDevice cp_device = VK_NULL_HANDLE;
VkCommandPool cp_commandPool= VK_NULL_HANDLE;


};