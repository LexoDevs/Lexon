#pragma once
#include "Core/VulkanContext.h"

#include <GLFWWindow.h>
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


class VulkanRHI 
{
public:
    VulkanRHI();

    void InitVulkan(Window& window);
    void InitRenderer();
    void UploadMesh(ObjectInstance& mesh);
    void DestroyVulkan();
    void DrawFrame(CameraView& camera,ObjectInstance& mesh, bool& UIVis);
    void recordCommandBuffer(uint32_t frame, uint32_t imageIndex, ObjectInstance mesh, bool& UIVisibility);


    //VulkanWindow&          GetVulkanWindow()          { return window; }
    VulkanInstance&        GetVulkanInstance()        { return instance; }
    //VulkanValidation&      GetVulkanValidation()      { return validacionlayers; }
    //VulkanSurface&         GetVulkanSurface()         { return surface; }
    VulkanPhysicalDevice&  GetVulkanPhysicalDevice()  { return physicaldevice; }
    VulkanLogicalDevice&   GetVulkanLogicalDevice()   { return device; }
    VulkanPipeline&        GetVulkanPipelineGraph()   { return pipeline; }
    VulkanCommandPool&     GetVulkanCommandPool()     { return commandpool;};
    VulkanDescriptorPool&  GetVulkanDescriptorPool()  { return descriptorpool;};
    VulkanDescriptorPool&  GetLayerDescriptorPool()  { return layerdescriptorpool;};

    CommandBuffer& GetVulkanCommandBuffer() {return commandBuffers;};
    VulkanSwapchain& GetVulkanSwapchain() {return swapchain;};

        UniformBuffer&  GetUniformBuffer()  { return uniformBuffer;};

    VulkanTexture& GetVulkanTexture() {return texture;};
    VulkanFence&           GetVulkanFence()           { return fences;};

        uint32_t GetCurrentFrame() {return currentFrame;};

private:
void RecreateSwapchain(Window* window);
    Window* activeWindow = nullptr; 

    VulkanContext context;

        
    //VulkanWindow window;
    VulkanInstance instance;
    VulkanValidation validacionlayers;
    VulkanSurface surface;

    
    VulkanPhysicalDevice physicaldevice;
    VulkanLogicalDevice device;
    VulkanSwapchain swapchain;
    VulkanPipeline pipeline;
    VulkanCommandPool commandpool;
    VulkanDescriptorPool descriptorpool;
    VulkanDescriptorPool layerdescriptorpool;

    DepthBuffer depthBuffer;
    VulkanTexture texture;
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    UniformBuffer uniformBuffer;
    VulkanDescriptorSet descriptorSet;
    CommandBuffer commandBuffers;
    VulkanFence fences;


    uint32_t currentFrame = 0;
}; 