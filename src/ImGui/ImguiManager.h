#pragma once
#include <vulkan/vulkan.h>
#include "../Vulkan/Window.h"



class ImGuiManager final
{
public:
    ImGuiManager(Window& window, VkInstance instance, VkPhysicalDevice physicalDevice,
                 VkDevice device, VkQueue graphicsQueue, uint32_t imageCount,
                 VkFormat colorFormat, VkCommandBuffer commandPool);
    
    ~ImGuiManager();

    void NewFrame();
    void Render(VkCommandBuffer commandBuffer);
    void EndFrame();                    // Opcional según tu flujo

    // Para manejar resize
    void OnWindowResize();

private:
    void CreateDescriptorPool();

    Window& m_Window;

    VkDevice         m_Device = VK_NULL_HANDLE;
    VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
};
