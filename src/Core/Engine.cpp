#include "Engine.h"

#include <iostream>
#include <chrono> 
#include <thread>

void Engine::EventManager(float deltaTime){

    ImGuiIO& io = ImGui::GetIO();

        const float movementAmount = camera.movementSpeed * deltaTime;
        if(inputSystem.IsKeyPressed(KeyCode::Escape))
        {
            window.CloseWindow();
        }

        if(inputSystem.IsKeyPressed(KeyCode::H))
        {            
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
        }
        
        if(inputSystem.IsKeyPressed(KeyCode::W))
        {
            camera.MoverAdelante(movementAmount);
        }

        if(inputSystem.IsKeyPressed(KeyCode::D))
        {
                        camera.MoverIzquierda(movementAmount);

        }

        if(inputSystem.IsKeyPressed(KeyCode::S))
        {
            glm::vec3 dir1 = glm::vec3(1.0f, 0.0f, 0.0f);
                        camera.MoverAdelante(-movementAmount);

        }

        if(inputSystem.IsKeyPressed(KeyCode::A))
        {
            glm::vec3 dir2 = glm::vec3(0.0f, 0.0f, 1.0f);
            camera.MoverIzquierda(-movementAmount);
        }

        if(inputSystem.IsKeyPressed(KeyCode::Q))
        {
            glm::vec3 dir2 = glm::vec3(0.0f, -1.0f, 0.0f);
            camera.MoverArriba(movementAmount);
        }

        if(inputSystem.IsKeyPressed(KeyCode::E))
        {
            glm::vec3 dir2 = glm::vec3(0.0f, 1.0f, 0.0f);
            camera.MoverArriba(-movementAmount);
        }

    const bool leftMouseHeld = inputSystem.IsMouseButtonPressed(MouseButton::Left);

    if (leftMouseHeld){
        const float deltaX = static_cast<float>(inputSystem.GetMouseDeltaX());

        const float deltaY = static_cast<float>(inputSystem.GetMouseDeltay());

        camera.ProcessMouseMovement(
            deltaX,
            deltaY
        );

    }else{

    }



        if(inputSystem.IsKeyPressed(KeyCode::Unknown)){ }
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
    window.SetMouseCallback();
    window.SetMousePosition();
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

    double previousFrameTime = window.GetTime();
    int frameCount = 0;
    double fps = 0.0;

    while (!window.ShouldClose()){
        double currentFrameTime = window.GetTime();

        const double deltaTime = currentFrameTime - previousFrameTime;


        inputSystem.BeginFrame();

        window.PollEvents();

        layersUI.ImGui_NewFrame();

        // Aquí dibujamos la interfaz
        LoadUIPanels();

        EditorInputCapture capture = layersUI.GetInputCapture();

        EventManager(deltaTime);
        VulkanAPI.DrawFrame(camera, window.GetHUDVisibility() );   // ← Dentro hacemos recordimgui
        layersUI.ImGui_EndFrame();   // Para viewports

        frameCount++;

        //if (currentTime - previousFrameTime >= 1.0) {
        fps = 1 / deltaTime;
        previousFrameTime = currentFrameTime;

            
            //Cambiar titulo añadiendo los FPS
            std::string title = "Vulkan Engine - FPS: " + std::to_string(static_cast<int>(fps));
            window.SetWindowTitle(title.c_str());
        //}

        // Mostrar FPS en consola 
        //if (currentFrameTime - previousFrameTime >= 0.2) {  // Imprimir cada 2 segundos
            std::cout << "\rFPS: " << std::fixed << static_cast<int>(fps) << "    " << std::flush;
            previousFrameTime = currentFrameTime;
        //}
    }
        std::cout<<std::endl;

    vkDeviceWaitIdle(VulkanAPI.GetVulkanLogicalDevice().GetHandle());

};



void Engine::CleanEngine() {
	//VulkanAPI.DestroyVulkan();

	//VulkanAPI.DestroyWindowSistem();

};


