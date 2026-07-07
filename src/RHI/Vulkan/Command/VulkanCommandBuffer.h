#include "../Core/VulkanContext.h"
#include "../Buffers/VulkanVertexBuffer.h"
#include "../Helpers/VulkanConstants.h"
#include <iostream>
    
    class CommandBuffer : public BufferI {
        public:

        CommandBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createCommandBuffer();

        private:
            VulkanContext& m_Context;


    };