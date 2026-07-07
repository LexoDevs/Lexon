#include "Engine.h"
#include <iostream>


void Engine::runEngine() {

	InitEngine();
	MainLoopEngine();
	CleanEngine();
};

void Engine::StartDrawFrame(){


    /*render.drawFrame(
        structureDraw.ComandPoolstr,
        structureDraw.swapchainstr,
        structureDraw.pipelinestr,
        structureDraw.renderstr,
        structureDraw.vertexbufferstr,
        structureDraw.texturestr,
        structureDraw.depthbufferstr,  
        structureDraw.meshstr,
        structureDraw.camerastr
    );*/
};

void Engine::InitEngine() {


    VulkanAPI.InitWindowSistem();

    VulkanRHI* pVulkan = &VulkanAPI;
    glfwSetWindowUserPointer(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow, pVulkan);

    VulkanAPI.InitVulkan();

    mesh.AddObject(loader);

    VulkanAPI.InitPostLoadElements(mesh);

    layersUI.ImGui_Init(VulkanAPI);  

};

void Engine::MainLoopEngine() {


    double previousTime = glfwGetTime();
    int frameCount = 0;
    double fps = 0.0;

    while (!glfwWindowShouldClose(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow)){

        glfwSetKeyCallback(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow, GLFW_KeyCallback);
        glfwPollEvents();

    // ImGui NewFrame ANTES de DrawFrame
    layersUI.ImGui_NewFrame();

    // Aquí dibujas tu interfaz
    ImGui::ShowDemoWindow();
    layersUI.VentanaSuperior(VulkanAPI);

    VulkanAPI.DrawFrame(camera, mesh);   // ← Dentro hará RecordImGui

    layersUI.ImGui_EndFrame();   // Para viewports

        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0) {
            fps = frameCount / (currentTime - previousTime);
            previousTime = currentTime;
            frameCount = 0;
            //Cambiar titulo añadiendo los FPS
            std::string title = "Vulkan Engine - FPS: " + std::to_string(static_cast<int>(fps));
            glfwSetWindowTitle(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow, title.c_str());
        }

        // Mostrar FPS en consola 
        static double lastPrintTime = 0.0;
        if (currentTime - lastPrintTime >= 0.2) {  // Imprimir cada 2 segundos
            std::cout << "\rFPS: " << std::fixed << std::setprecision(1) << fps << "    " << std::flush;
            lastPrintTime = currentTime;
        }
    }
        std::cout<<std::endl;

    vkDeviceWaitIdle(VulkanAPI.GetVulkanContext().device);

};


void Engine::CleanEngine() {

	VulkanAPI.DestroyVulkan();

	VulkanAPI.DestroyWindowSistem();

};


