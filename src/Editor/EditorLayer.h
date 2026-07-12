#pragma once
 #include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"  
#include "../RHI/Vulkan/VulkanRHI.h"


class EditorLayer{

public:
void ImGui_Init(VulkanRHI& VulkanAPI);
void ImGui_NewFrame();
void ImGui_Render(VkCommandBuffer cmd);
void ImGui_EndFrame();
void VentanaSuperior(VulkanRHI& VulkanAPI);
void MuestreoImagenes(VulkanRHI& VulkanAPI);
};
