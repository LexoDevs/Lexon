#pragma once
#include "VulkanVertexBuffer.h"
#include "../../../Renderer/Camera.h"
#include "../../../Renderer/RenderSettings.h"
#include "../../../Renderer/Lighting/DirectionalLight.h"


    class UniformBuffer : public VertexBuffer {
        public:
        UniformBuffer();
        ~UniformBuffer();

            void createUniformBuffer(VkDevice device, VkPhysicalDevice physicaldevice);
            void destroyBuffer() override;
            void updateUniformBuffer(
                uint32_t currentImage,
                CameraView camera,
                VkExtent2D swapChainExtent,
                const RenderSettings& renderSettings,
                const DirectionalLight& sunLight

            );            
VkBuffer* GetUniformBuffer() { return uniformBuffers;};


        private:
    VkBuffer uniformBuffers[MAX_FRAMES_IN_FLIGHT];
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    UniformBufferObject UBO{};

//VkDevice cp_device;


    };

    