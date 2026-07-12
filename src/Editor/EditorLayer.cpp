


#include "EditorLayer.h"


// Asumiendo que tienes acceso a tu VkInstance, VkDevice, etc.
void EditorLayer::ImGui_Init(VulkanRHI& VulkanAPI)
{
    VulkanContext context = VulkanAPI.GetVulkanContext();   // Recomiendo pasar por referencia
    context.imguiDescriptorPool = VulkanAPI.GetVulkanDescriptorPool().CreateImGuiDescriptorPool();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   
    io.ConfigFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::StyleColorsDark();

    // Platform backend
    ImGui_ImplGlfw_InitForVulkan(context.GLFWwindow, true);

    // Renderer backend (Vulkan)
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance        = context.instance;
    init_info.PhysicalDevice  = context.physicalDevice;
    init_info.Device          = context.device;
    init_info.QueueFamily     = context.QueueFamilie.graphicsFamily.value();  // ← importante usar graphics
    init_info.Queue           = context.graphicsQueue;                // ← NO uses transferQueue
    init_info.PipelineCache   = VK_NULL_HANDLE;
    init_info.DescriptorPool  = context.imguiDescriptorPool;
    init_info.MinImageCount   = 2;
    init_info.ImageCount      = (uint32_t)context.swapchainImages.size();
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.UseDynamicRendering = true;
    
    VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo = {};
    pipelineRenderingInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipelineRenderingInfo.colorAttachmentCount    = 1;
    pipelineRenderingInfo.pColorAttachmentFormats = &context.swapChainImageFormat;

        init_info.PipelineInfoMain.PipelineRenderingCreateInfo = pipelineRenderingInfo;

    // Opcional pero muy útil para debug
    // init_info.CheckVkResultFn = check_vk_result_function;

    // Inicializar Vulkan backend
    if (!ImGui_ImplVulkan_Init(&init_info))
    {
        std::cerr << "ImGui_ImplVulkan_Init failed!\n";
    }

    VkCommandBuffer cmd = VulkanAPI.GetVulkanContext().commandBuffer;
    //ImGui_ImplVulkan_CreateFontsTexture(cmd);
}

void EditorLayer::ImGui_NewFrame()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //Esto activa el bindeo a los extremos de las ventanas
    ImGuiViewport* dockspace_id = ImGui::GetMainViewport();
    ImGui::DockSpaceOverViewport(0, dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode);
}

void EditorLayer::ImGui_Render(VkCommandBuffer cmd)
{
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}

// Al final del frame (para viewports)
void EditorLayer::ImGui_EndFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void EditorLayer::VentanaSuperior(VulkanRHI& VulkanAPI){

    // === BARRA SUPERIOR CLÁSICA ===
if (ImGui::BeginMainMenuBar())
{
    // Color de fondo gris oscuro + bordes azules
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));   // Gris oscuro
    ImGui::PushStyleColor(ImGuiCol_Border,     ImVec4(0.0f, 0.6f, 1.0f, 1.0f));     // Azul
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);   // Bordes visibles

    // Menú File
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New"))     {}
        if (ImGui::MenuItem("Open"))    {}
        if (ImGui::MenuItem("Save"))    {}
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))    { /* glfwSetWindowShouldClose(...); */ }
        ImGui::EndMenu();
    }

    // Menú Edit
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo")) {}
        if (ImGui::MenuItem("Redo")) {}
        ImGui::EndMenu();
    }

    // Menú View
    if (ImGui::BeginMenu("View"))
    {
        //ImGui::MenuItem("Show Demo Window", nullptr, &show_demo_window, 0);
        ImGui::EndMenu();
    }

    // Menú Help
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("About")) {}
        ImGui::EndMenu();
    }

    // === Botón de cierre a la derecha ===
    ImGui::SameLine(ImGui::GetWindowWidth() - 50);  // Ajusta posición

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));      // Rojo
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));

    if (ImGui::Button("X", ImVec2(40, 20)))
    {
        // Cierra la aplicación
        glfwSetWindowShouldClose(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow, GLFW_TRUE);
    }

    ImGui::PopStyleColor(2);  // Pop del botón

    ImGui::PopStyleVar();     // FrameBorderSize
    ImGui::PopStyleColor(2);  // MenuBarBg + Border

    ImGui::EndMainMenuBar();
}
};

void EditorLayer::MuestreoImagenes(VulkanRHI& VulkanAPI){

    ImGui::Begin("Vulkan Texture Test");
    ImGui::Text("pointer = %p", VulkanAPI.GetVulkanContext().textureImage);
    ImGui::Text("size = %d x %d", VulkanAPI.GetVulkanContext().texWidth, VulkanAPI.GetVulkanContext().texHeight);
    //ImGui::Image((ImTextureID)VulkanAPI.GetVulkanContext().textureImage, ImVec2(VulkanAPI.GetVulkanContext().texWidth, VulkanAPI.GetVulkanContext().texHeight));
    ImGui::End();

};

