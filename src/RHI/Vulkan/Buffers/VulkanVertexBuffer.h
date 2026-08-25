#pragma once
#include "../Core/VulkanContext.h"
#include "../Helpers/VulkanConstants.h"
#include <iostream>
#include "../../../Assets/Loaders/LoaderAssets.h"

#include "VulkanBuffer.h"


//------------------------------HIJOS--------------------------//

    class VertexBuffer : public BufferI {
        public:
        VertexBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createVertexBuffer(ObjectInstance& mesh);
            void destroyVertexBuffer();
            void setVertexBuffer(VkBuffer new_vertexbuffer){  m_Context.vertexBuffer = new_vertexbuffer;}

        private:
            VulkanContext& m_Context;



    };









