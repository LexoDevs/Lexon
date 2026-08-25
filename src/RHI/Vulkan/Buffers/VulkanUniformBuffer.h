#pragma once
#include "VulkanBuffer.h"
#include "../../../Renderer/Camera.h"

    class UniformBuffer : public BufferI {
        public:
        UniformBuffer(VulkanContext& context)
        : BufferI(context),
        m_Context(context)              
    {
    }

            void createUniformBuffer();
            void destroyUniformBuffer();
            void updateUniformBuffer(uint32_t currentImage, ObjectInstance mesh, CameraView camera);

        private:
            VulkanContext& m_Context;



    };

    