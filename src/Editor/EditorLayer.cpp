

#include "EditorLayer.h"

EditorLayer::~EditorLayer(){
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

};

// Asumiendo que tienes acceso a tu VkInstance, VkDevice, etc.
void EditorLayer::ImGui_Init(VulkanRHI& VulkanAPI, void* window)
{
    GLFWwindow* cp_Window = static_cast<GLFWwindow*>(window); 

    

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   
    io.ConfigFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::StyleColorsDark();

    // Platform backend
    ImGui_ImplGlfw_InitForVulkan(cp_Window, true);

    // Renderer backend (Vulkan)
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance        = VulkanAPI.GetVulkanInstance().GetInstance();
    init_info.PhysicalDevice  = VulkanAPI.GetVulkanPhysicalDevice().GetPhysicalDevice();
    init_info.Device          = VulkanAPI.GetVulkanLogicalDevice().GetHandle();
    init_info.QueueFamily     = VulkanAPI.GetVulkanLogicalDevice().GetQueueFamily().graphicsFamily.value(); 
    init_info.Queue           = VulkanAPI.GetVulkanLogicalDevice().GetGraphicsQueue();        
    init_info.PipelineCache   = VK_NULL_HANDLE;
    init_info.DescriptorPool  = VulkanAPI.GetLayerDescriptorPool().GetDescriptorPool();
    init_info.MinImageCount   = 2;
    init_info.ImageCount      = (uint32_t)VulkanAPI.GetVulkanSwapchain().GetSwapchainImagesRef().size();
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.UseDynamicRendering = true;
    
    VkPipelineRenderingCreateInfoKHR pipelineRenderingInfo = {};
    pipelineRenderingInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
    pipelineRenderingInfo.colorAttachmentCount    = 1;

    
    VkFormat swapimageformat_p = VulkanAPI.GetVulkanSwapchain().GetSwapChainImageFormat();
    pipelineRenderingInfo.pColorAttachmentFormats = &swapimageformat_p;

        init_info.PipelineInfoMain.PipelineRenderingCreateInfo = pipelineRenderingInfo;

    // Opcional pero muy útil para debug
    // init_info.CheckVkResultFn = check_vk_result_function;

    // Inicializar Vulkan backend
    if (!ImGui_ImplVulkan_Init(&init_info))
    {
        std::cerr << "ImGui_ImplVulkan_Init failed!\n";
    }

    //VkCommandBuffer cmd = VulkanAPI.GetVulkanCommandBuffer().GetCommandBuffer(VulkanAPI.GetCurrentFrame());
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
        if (ImGui::MenuItem("New"))     {
            MeshesList cube = {"../src/Meshes/cube.obj" , "Cubo"};
            LoaderList.emplace_back(cube);
            
        
        }
        if (ImGui::MenuItem("Open"))    {}
        if (ImGui::MenuItem("Save"))    {}
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))    { /*glfwSetWindowShouldClose(Window.().GLFWwindow,GLFW_TRUE);*/ }
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
        //glfwSetWindowShouldClose(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow, GLFW_TRUE);
    }

    ImGui::PopStyleColor(2);  // Pop del botón

    ImGui::PopStyleVar();     // FrameBorderSize
    ImGui::PopStyleColor(2);  // MenuBarBg + Border

    ImGui::EndMainMenuBar();
}
};

    static int seleccionado = 0; 

void EditorLayer::MuestreoImagenes(VulkanRHI& VulkanAPI){
    

    ImGui::Begin("Vulkan Texture Test");

    for (size_t i = 0; i < TEXTURE_PATHS.size(); i++){

        bool es_seleccionado = (seleccionado == i);

        if (ImGui::Selectable(TEXTURE_PATHS[i].data(), es_seleccionado))
        {
            std::cout<<"Textura seleccionada: "<<i<<std::endl;
            seleccionado = i;

        }
        if (es_seleccionado){

            ImGui::SetItemDefaultFocus();
        }

    }


    //ImGui::Text("pointer = %p", VulkanAPI.GetVulkanContext().textureImageView);
    //ImGui::Text("size = %d x %d", VulkanAPI.GetVulkanContext().texWidth[seleccionado], VulkanAPI.GetVulkanContext().texHeight[seleccionado]);

    //ImGui::Image(VulkanAPI.GetVulkanContext().descriptorSets[seleccionado], ImVec2(VulkanAPI.GetVulkanContext().texWidth[seleccionado]/5, VulkanAPI.GetVulkanContext().texHeight[seleccionado]/5));

   ImGui::End();
};




void EditorLayer::ElementosEnEscena(const CpuModel& model)
{
    ImGui::Begin("Hierarchy");

    ImGui::Text(
        "Model: %s",
        model.sourcePath.filename().string().c_str()
    );

    ImGui::Text(
        "%zu meshes | %zu materials",
        model.meshes.size(),
        model.materialNames.size()
    );

    ImGui::Separator();

    constexpr ImGuiTableFlags tableFlags =
        ImGuiTableFlags_BordersV |
        ImGuiTableFlags_BordersOuterH |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_NoBordersInBody |
        ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable(
            "SceneHierarchyTable",
            3,
            tableFlags,
            ImVec2(0.0f, 0.0f)))
    {
        ImGui::TableSetupColumn(
            "Name",
            ImGuiTableColumnFlags_NoHide |
            ImGuiTableColumnFlags_WidthStretch
        );

        ImGui::TableSetupColumn(
            "Size",
            ImGuiTableColumnFlags_WidthFixed,
            120.0f
        );

        ImGui::TableSetupColumn(
            "Type / Material",
            ImGuiTableColumnFlags_WidthFixed,
            180.0f
        );

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableHeadersRow();

        DrawNode(model.rootNode, model);

        ImGui::EndTable();
    }

    ImGui::End();
}

void EditorLayer::DrawNode(
    const CpuNode& node,
    const CpuModel& model)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);

    const bool hasChildren =
        !node.children.empty() ||
        !node.meshIndices.empty();

    ImGuiTreeNodeFlags nodeFlags =
        ImGuiTreeNodeFlags_SpanAllColumns |
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (!hasChildren)
    {
        nodeFlags |=
            ImGuiTreeNodeFlags_Leaf |
            ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    ImGui::PushID(&node);

    const bool nodeOpen = ImGui::TreeNodeEx(
        "Node",
        nodeFlags,
        "%s",
        node.name.empty() ? "Unnamed Node" : node.name.c_str()
    );

    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%zu", node.meshIndices.size());

    ImGui::TableSetColumnIndex(2);
    ImGui::TextUnformatted("Node");

    if (nodeOpen && hasChildren)
    {
        for (uint32_t meshIndex : node.meshIndices)
        {
            if (meshIndex >= model.meshes.size())
            {
                continue;
            }

            const CpuMesh& mesh = model.meshes[meshIndex];

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            ImGui::PushID(static_cast<int>(meshIndex));

            const bool selected = static_cast<int32_t>(meshIndex);

            const std::string meshName =
                mesh.name.empty()
                    ? "Unnamed Mesh"
                    : mesh.name;

            if (ImGui::Selectable(
                    meshName.c_str(),
                    selected,
                    ImGuiSelectableFlags_SpanAllColumns))
            {
                //selectedMeshIndex =
                    static_cast<int32_t>(meshIndex);
            }

            ImGui::TableSetColumnIndex(1);
            ImGui::Text(
                "%zu V / %zu I",
                mesh.vertices.size(),
                mesh.indices.size()
            );

            ImGui::TableSetColumnIndex(2);

            if (mesh.materialIndex < model.materialNames.size())
            {
                ImGui::Text(
                    "Mesh | %s",
                    model.materialNames[mesh.materialIndex].c_str()
                );
            }
            else
            {
                ImGui::Text(
                    "Mesh | Material %u",
                    mesh.materialIndex
                );
            }

            ImGui::PopID();
        }

        for (const CpuNode& child : node.children)
        {
            DrawNode(child, model);
        }

        ImGui::TreePop();
    }

    ImGui::PopID();
}