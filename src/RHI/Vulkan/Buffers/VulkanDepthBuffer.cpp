#include "VulkanDepthBuffer.h"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


void DepthBuffer::createDepthFormat(){

        m_Context.depthformat = findDepthFormat();


}

void DepthBuffer::createDepthResources(){
        createDepthFormat();
        
        m_Context.depthsize.width  = m_Context.swapChainExtent.width;
    m_Context.depthsize.height = m_Context.swapChainExtent.height;

        createImage(m_Context.depthsize.width, m_Context.depthsize.height, m_Context.depthformat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context.depthImage, m_Context.depthImageMemory, 1);
        m_Context.depthImageView = createImageView(m_Context.depthImage, m_Context.depthformat, VK_IMAGE_ASPECT_DEPTH_BIT, 1 );
        
};


VkFormat DepthBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_Context.physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                m_Context.depthformat = format;
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                m_Context.depthformat = format;
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
    }


VkFormat DepthBuffer::findDepthFormat() {
    return findSupportedFormat(
    {VK_FORMAT_D32_SFLOAT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}


bool DepthBuffer::hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

void DepthBuffer::destroyDepthResources(){

    vkDestroyImageView(m_Context.device, m_Context.depthImageView, nullptr);
    vkDestroyImage(m_Context.device, m_Context.depthImage, nullptr);
    vkFreeMemory(m_Context.device, m_Context.depthImageMemory, nullptr);
};

void DepthBuffer::cleanDepthResources(){

    vkDestroyImage(m_Context.device, m_Context.depthImage, nullptr);

};













void DepthBuffer::recordCommandBuffer(uint32_t imageIndex, uint32_t currentFrame, ObjectInstance mesh)
{
    VkCommandBuffer cmd = m_Context.commandBuffers[currentFrame];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(cmd, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    

    transition_image_layout(
        cmd,
        m_Context.swapchainImages[imageIndex],
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,                                                // srcAccessMask
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,             // srcStage
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT  // dstStage
    );
        
    // Transición del Depth Image
    transition_image_layout(
        cmd,
        m_Context.depthImage,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
        VK_IMAGE_ASPECT_DEPTH_BIT,
        VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
        VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT
    );

        // ====================== INICIO DEL RENDERING ======================
    VkClearValue clearValues[2] = {};
    clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderingAttachmentInfo colorAttachment{};
    colorAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    colorAttachment.imageView   = m_Context.swapChainImageViews[imageIndex];
    colorAttachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.clearValue  = clearValues[0];



    VkRenderingAttachmentInfo depthAttachment{};
    depthAttachment.sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.imageView   = m_Context.depthImageView;
    
    depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    depthAttachment.loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp     = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.clearValue  = clearValues[1];


    VkRenderingInfo renderingInfo{};
    renderingInfo.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea.offset    = {0, 0};



    renderingInfo.renderArea.extent    = {m_Context.swapChainExtent.width,m_Context.swapChainExtent.height};

    renderingInfo.layerCount           = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments    = &colorAttachment;
    renderingInfo.pDepthAttachment     = &depthAttachment;

    

    vkCmdBeginRendering(cmd, &renderingInfo);

    // Bind del pipeline gráfico
    vkCmdBindPipeline(m_Context.commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_Context.Pipeline);

    vkCmdSetDepthTestEnable(cmd, VK_TRUE);
vkCmdSetDepthWriteEnable(cmd, VK_TRUE);
vkCmdSetDepthCompareOp(cmd, VK_COMPARE_OP_LESS);

vkCmdSetDepthBiasEnable(cmd, VK_FALSE);
vkCmdSetDepthBias(cmd, 0.0f, 0.0f, 0.0f);

vkCmdSetStencilTestEnable(cmd, VK_FALSE);

vkCmdSetDepthBoundsTestEnable(cmd, VK_FALSE);
vkCmdSetDepthBounds(cmd, 0.0f, 1.0f);

    // Viewport y Scissor (dynamic state)
    VkViewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = static_cast<float>(1280);//window.width
    viewport.height   = static_cast<float>(720);//window.height
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    // Scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_Context.swapChainExtent;

    vkCmdSetViewport(m_Context.commandBuffers[currentFrame], 0, 1, &viewport); //??
    vkCmdSetScissor(cmd, 0, 1, &scissor);


    VkBuffer vertexBuffers[] = { m_Context.vertexBuffer };
    VkDeviceSize offsets[]   = { 0 };
    
    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cmd, m_Context.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(cmd, 
                        VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        m_Context.pipelineLayout, 
                        0, 1, 
                        &m_Context.descriptorSets[currentFrame],   // Asumiendo que es un método de Texture
                        0, nullptr);

    vkCmdDrawIndexed(cmd, static_cast<uint32_t>(mesh.getIndices().size()), 1, 0, 0, 0);

    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);

    vkCmdEndRendering(cmd);


    transition_image_layout(
        cmd,
        m_Context.swapchainImages[imageIndex],
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
        0,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT
    );

    if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

