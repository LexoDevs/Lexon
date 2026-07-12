#pragma once
#include "Core/VulkanContext.h"

#include "Windows/VulkanWindow.h"
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
#include "Resources/VulkanTexture.h"

#include "Descriptors/VulkanDescriptorSet.h"
#include "Command/VulkanCommandBuffer.h"

#include "Sync/VulkanFence.h"

#include "Render/VulkanRender.h"


class VulkanRHI 
{
public:
    VulkanRHI();


    void InitWindowSistem();
    void DestroyWindowSistem();
    void InitVulkan();
    void InitPostLoadElements(ObjectInstance& mesh);
    void DestroyVulkan();
    void DrawFrame(CameraView& camera,ObjectInstance& mesh);


    VulkanContext& GetVulkanContext() { return context; };

    VulkanWindow&          GetVulkanWindow()          { return window; }
    VulkanInstance&        GetVulkanInstance()        { return instance; }
    VulkanValidation&      GetVulkanValidation()      { return validacionlayers; }
    VulkanSurface&         GetVulkanSurface()         { return surface; }
    VulkanPhysicalDevice&  GetVulkanPhysicalDevice()  { return physicaldevice; }
    VulkanLogicalDevice&   GetVulkanLogicalDevice()   { return device; }
    VulkanPipeline&        GetVulkanPipelineGraph()   { return pipeline; }
    VulkanCommandPool&     GetVulkanCommandPool()     { return commandpool;};
    VulkanDescriptorPool&  GetVulkanDescriptorPool()  { return descriptorpool;};
    
        UniformBuffer&  GetUniformBuffer()  { return uniformBuffer;};

    VulkanTexture& GetVulkanTexture() {return texture;};
    VulkanFence&           GetVulkanFence()           { return fences;};

private:
    VulkanContext context;

        
    VulkanWindow window;
    VulkanInstance instance;
    VulkanValidation validacionlayers;
    VulkanSurface surface;
    VulkanPhysicalDevice physicaldevice;
    VulkanLogicalDevice device;
    VulkanSwapchain swapchain;
    VulkanPipeline pipeline;
    VulkanCommandPool commandpool;
    VulkanDescriptorPool descriptorpool;
    DepthBuffer depthBuffer;
    VulkanTexture texture;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    UniformBuffer uniformBuffer;
    VulkanDescriptorSet descriptorSet;
    CommandBuffer commandBuffer;
    VulkanFence fences;

}; 