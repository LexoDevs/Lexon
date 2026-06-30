#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

// Asumiendo que tienes acceso a tu VkInstance, VkDevice, etc.
void ImGui_Init(GLFWwindow* window, VkInstance instance, VkDevice device, 
                VkPhysicalDevice physicalDevice, VkQueue queue, 
                uint32_t imageCount, VkFormat swapchainFormat)
{
    // Configuración de contexto
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   

    // Estilo (opcional)
    ImGui::StyleColorsDark();

    // Inicializar backends
    ImGui_ImplGlfw_InitForVulkan(window, true);

    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = instance;
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = device;
    init_info.QueueFamily = /* queue family */;
    init_info.Queue = queue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = /*  descriptor pool para imgui */;
    init_info.MinImageCount = 2;
    init_info.ImageCount = imageCount;
    init_info.UseDynamicRendering = true; 

    init_info.PipelineInfoMain.Subpass = 0;
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    /*

    uint32_t                        QueueFamily;
    VkDescriptorPool                DescriptorPool;  
   
        
    */ 

    // Subir fuentes
}

void ImGui_NewFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGui_Render(VkCommandBuffer cmd)
{
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}

// Al final del frame (para viewports)
void ImGui_EndFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}