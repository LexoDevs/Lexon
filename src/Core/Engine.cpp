#include "Engine.h"
#include <iostream>


void Engine::runEngine() {

	InitEngine();
    std::cout<<"BENCHMARK INICIO DEL ENGINE"<<std::endl;

	MainLoopEngine();
    std::cout<<"BENCHMARK FIN DEL LOOP DEL ENGINE"<<std::endl;

	CleanEngine();
    std::cout<<"BENCHMARK FIN DE LIMPIEZA Y CIERRE"<<std::endl;

};


void Engine::InitEngine() {

    window.InitWindow();

    //VulkanRHI* pVulkan = &VulkanAPI;
    //glfwSetWindowUserPointer(window, pVulkan);
/*
    VulkanAPI.InitVulkan();

    mesh.AddObject(loader);

    VulkanAPI.InitPostLoadElements(mesh);

    layersUI.ImGui_Init(VulkanAPI);  
*/
};

/*
funciones a recrear:
        glfwSetKeyCallback(glfwwindow, GLFW_KeyBoardCallback);


*/


void Engine::MainLoopEngine() {

    //Seleccion de ventana

    double previousTime = window.GetTime();
    int frameCount = 0;
    double fps = 0.0;

    while (!window.ShouldClose()){
        window.SetKeyCallback();
        window.PoolEvents();
        //glfwSetMouseButtonCallback(VulkanAPI.GetVulkanWindow().getContext().GLFWwindow, GLFW_MouseButtonCallback);  // ← Añadir esta línea
        //glfwPollEvents();

        /*
        layersUI.ImGui_NewFrame();
        // Aquí dibujamos la interfaz
        ImGui::ShowDemoWindow();
        layersUI.VentanaSuperior(VulkanAPI);
        layersUI.MuestreoImagenes(VulkanAPI);
        layersUI.ElementosEnEscena(VulkanAPI);
        VulkanAPI.DrawFrame(camera, mesh);   // ← Dentro hacemos recordimgui


        layersUI.ImGui_EndFrame();   // Para viewports

        */

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

    //vkDeviceWaitIdle(VulkanAPI.GetVulkanContext().device);

};



void Engine::CleanEngine() {

	//VulkanAPI.DestroyVulkan();

	VulkanAPI.DestroyWindowSistem();

};


