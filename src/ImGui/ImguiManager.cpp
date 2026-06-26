#include "ImGuiManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <stdexcept>



ImGuiManager::ImGuiManager(Window& window, VkInstance instance, 
    VkPhysicalDevice physicalDevice, VkDevice device, VkQueue graphicsQueue,
    uint32_t imageCount, VkFormat colorFormat, VkCommandBuffer commandPool)
    : m_Window(window), m_Device(device)
{
    // Configuración de estilo (opcional)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Activa docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Opcional (más avanzado)

    ImGui::StyleColorsDark();

    // Inicializar backends
    ImGui_ImplGlfw_InitForVulkan(window.GetWindows(0), true);

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance;
    initInfo.PhysicalDevice = physicalDevice;
    initInfo.Device = device;
    initInfo.QueueFamily = 0;           // Ajusta según tu queue family
    initInfo.Queue = graphicsQueue;
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = VK_NULL_HANDLE; // Lo creamos manualmente
    initInfo.Subpass = 0;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = imageCount;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    initInfo.UseDynamicRendering = true;   // Muy recomendado con tu enfoque futuro

    CreateDescriptorPool();

    initInfo.DescriptorPool = m_DescriptorPool;

    ImGui_ImplVulkan_Init(nullptr); // nullptr si usas Dynamic Rendering

    // Subir fuentes (font)
    VkCommandBuffer cmd = commandPool;
    //ImGui_ImplVulkan_CreateFontsTexture(cmd);
    // ...
}

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_DescriptorPool)
        vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
}

void ImGuiManager::CreateDescriptorPool()
{
    VkDescriptorPoolSize poolSizes[] = {
        { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
        // ... otros tipos
    };

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    poolInfo.maxSets = 1000;
    poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
    poolInfo.pPoolSizes = poolSizes;

    if (vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create ImGui descriptor pool");
}

void ImGuiManager::NewFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render(VkCommandBuffer commandBuffer)
{
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}
