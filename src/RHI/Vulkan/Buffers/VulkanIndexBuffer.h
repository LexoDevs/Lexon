#pragma once
#include "VulkanBuffer.h"    
    
    class IndexBuffer : public BufferI {
        public:
        IndexBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createIndexBuffer(ObjectInstance& mesh);
            void destroyIndexBuffer();

        private:
            VulkanContext& m_Context;


    };
