#pragma once

#include "../../Assets/Importer/CPUModel.h"


#include "Instance/VulkanInstance.h"
#include "Windows/VulkanSurface.h"
#include "Instance/ValidationLayers.h"
#include "Swapchain/VulkanSwapchain.h"
#include "Device/VulkanLogDevice.h"
#include "Device/VulkanPhyDevice.h"
#include "Pipeline/VulkanPipeline.h"
#include "Descriptors/VulkanDescriptorPool.h"
#include "Command/VulkanCommandPool.h"
#include "Buffers/VulkanVertexBuffer.h"
#include "Buffers/VulkanIndexBuffer.h"
#include "Buffers/VulkanUniformBuffer.h"
#include "Buffers/VulkanDepthBuffer.h"
#include "Resources/VulkanTexture.h"
#include "Descriptors/VulkanDescriptorSet.h"
#include "Command/VulkanCommandBuffer.h"
#include "Sync/VulkanFence.h"
#include "Render/VulkanRender.h"

#include <GLFWWindow.h>

//#include "../../Core/WindowSystem"

class VulkanRHI 
{
public:
    VulkanRHI();

    void InitVulkan(Window& window);
    void InitRenderer(const CpuModel& model);
    void UploadMesh(CpuModel& mesh);
    void DestroyVulkan();
    void DrawFrame(CameraView& camera, bool& UIVis);
    void recordCommandBuffer(uint32_t frame, uint32_t imageIndex, bool& UIVisibility);


    VulkanInstance&        GetVulkanInstance()        { return instance; }
    VulkanPhysicalDevice&  GetVulkanPhysicalDevice()  { return physicaldevice; }
    VulkanLogicalDevice&   GetVulkanLogicalDevice()   { return device; }
    VulkanSurface&         GetVulkanSurface()         { return surface; }

    VulkanPipeline&        GetVulkanPipelineGraph()   { return pipeline; }
    VulkanCommandPool&     GetVulkanCommandPool()     { return commandpool;};
    VulkanDescriptorPool&  GetVulkanDescriptorPool()  { return descriptorpool;};
    VulkanDescriptorPool&  GetLayerDescriptorPool()   { return layerdescriptorpool;};
    
    CommandBuffer&         GetVulkanCommandBuffer()   { return commandBuffers;};
    VulkanSwapchain&       GetVulkanSwapchain()       { return swapchain;};
    UniformBuffer&         GetUniformBuffer()         { return uniformBuffer;};
    VulkanTexture&         GetVulkanTexture()         { return texture;};
    VulkanFence&           GetVulkanFence()           { return fences;};
    VulkanDescriptorSet&   GetDescriptorSet()         { return descriptorSet;};
    VertexBuffer&          GetVertexBuffer()          { return vertexBuffer;};
    IndexBuffer&           GetIndexBuffer()           { return indexBuffer;};
    DepthBuffer&           GetDepthBuffer()           { return depthBuffer;};

    uint32_t               GetCurrentFrame()          { return currentFrame;};


private:
    void RecreateSwapchain(Window* window);

    Window* activeWindow = nullptr; 


    VulkanInstance instance;
    VulkanPhysicalDevice physicaldevice;
    VulkanLogicalDevice device;
    VulkanSurface surface;

    VulkanPipeline pipeline;
    VulkanCommandPool commandpool;
    VulkanDescriptorPool descriptorpool;
    VulkanDescriptorPool layerdescriptorpool;

    CommandBuffer commandBuffers;
    VulkanSwapchain swapchain;
    UniformBuffer uniformBuffer;
    VulkanTexture texture;
    VulkanFence fences;

    VulkanDescriptorSet descriptorSet;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;

    DepthBuffer depthBuffer;

    uint32_t currentFrame = 0;
    std::vector<CpuMaterial> materials;
}; 