#include "Engine.h"

#include <iostream>
#include <chrono> 
#include <thread>

void Engine::EventManager(){

    KeyCode keyselected =inputSystem.Selector();


    switch(keyselected){

        case KeyCode::Escape:

            window.CloseWindow();
        break;

        case KeyCode::W:
            if (window.GetHUDVisibility()==true)
            {
                window.SetHUDVisibility(false);
                std::cout<<"Visibilidad de HUD:"<<window.GetHUDVisibility()<<std::endl;
            }
            else
            {
                window.SetHUDVisibility(true);
                std::cout<<"Visibilidad de HUD:"<<window.GetHUDVisibility()<<std::endl;

            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        break;

        //case KeyCode::E:
        //    glm::vec3 in = glm::vec3(0.0f, 0.0f, 0.0f);
        //    camera.alante();
        //break;
    }


};


void Engine::runEngine() {

	InitEngine();

	MainLoopEngine();

	CleanEngine();

};

void Engine::InitEngine() {

    window.SetInputSystem(&inputSystem);
    window.InitWindow();
    window.SetKeyCallback();
    VulkanAPI.InitVulkan(window);

    std::filesystem::path path = "../resources/models/sponza.obj";
    model = assimploader.Load(path);

    VulkanAPI.InitRenderer(model);

    VulkanAPI.UploadMesh(model);
    layersUI.ImGui_Init(VulkanAPI, window.GetNativeWindow());  
};


void Engine::LoadUIPanels() {
        ImGui::ShowDemoWindow();
        layersUI.VentanaSuperior(VulkanAPI);
        layersUI.MuestreoImagenes(VulkanAPI);
        layersUI.ElementosEnEscena(model);

        ImGui::Render();

};


void Engine::MainLoopEngine() {

    //Seleccion de ventana

    double previousTime = window.GetTime();
    int frameCount = 0;
    double fps = 0.0;

    while (!window.ShouldClose()){

        window.PollEvents();
        EventManager();

        layersUI.ImGui_NewFrame();
        // Aquí dibujamos la interfaz
        LoadUIPanels();

        VulkanAPI.DrawFrame(camera, window.GetHUDVisibility() );   // ← Dentro hacemos recordimgui
        layersUI.ImGui_EndFrame();   // Para viewports


        double currentTime = window.GetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0) {
            fps = frameCount / (currentTime - previousTime);
            previousTime = currentTime;
            frameCount = 0;
            
            //Cambiar titulo añadiendo los FPS
            std::string title = "Vulkan Engine - FPS: " + std::to_string(static_cast<int>(fps));
            window.SetWindowTitle(title.c_str());
        }

        // Mostrar FPS en consola 
        static double lastPrintTime = 0.0;
        if (currentTime - lastPrintTime >= 0.2) {  // Imprimir cada 2 segundos
            std::cout << "\rFPS: " << std::fixed << std::setprecision(1) << fps << "    " << std::flush;
            lastPrintTime = currentTime;
        }
    }
        std::cout<<std::endl;

    vkDeviceWaitIdle(VulkanAPI.GetVulkanLogicalDevice().GetHandle());

};



void Engine::CleanEngine() {
	//VulkanAPI.DestroyVulkan();

	//VulkanAPI.DestroyWindowSistem();

};


