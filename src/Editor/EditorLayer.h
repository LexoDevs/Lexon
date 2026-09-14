#pragma once
 #include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"  
#include "implot.h"

#include "../RHI/Vulkan/VulkanRHI.h"
#include "../Renderer/RenderObject.h"
#include "EditorSelection.h"
#include <cstdint>
#include <vector>

struct EditorInputCapture
{
    bool mouse = false;
    bool keyboard = false;
    bool text = false;
};

class EditorLayer{

    public:

        ~EditorLayer();
        EditorInputCapture GetInputCapture() const;
        void ImGui_Init(VulkanRHI& VulkanAPI, void* window);
        void ImGui_NewFrame();
        void ImGui_Render(VkCommandBuffer cmd);
        void ImGui_EndFrame();
        void VentanaSuperior(VulkanRHI& VulkanAPI);
        void MuestreoImagenes(VulkanRHI& VulkanAPI);
        void ElementosEnEscena(
            std::vector<RenderObject>& objects,
            EditorSelection& selection
        );
        void DrawNode(const CpuNode& node, const CpuModel& model);
        void LogFPS(double fps,double time);
    private:
};


