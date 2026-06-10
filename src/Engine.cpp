#include "Engine.h"

void Engine::runEngine() {

	InitEngine();
	MainLoopEngine();
	CleanEngine();
};

void Engine::StartDrawFrame(infoDraw& structureDraw){

    render.drawFrame(
        structureDraw.logicaldevicesstr,
        structureDraw.ComandPoolstr,
        structureDraw.swapchainstr,
        structureDraw.pipelinestr,
        structureDraw.windowsurfacestr,
        structureDraw.physicaldevicestr,
        structureDraw.windowstr
    );
};

void Engine::InitEngine() {

	window.InitWindowsSistem();
	instance.CreateInstance();
    windowsurface.CreateWindowSurface(instance, window);
	physicaldevice.SelectPhysicalDevices(instance);
	logicaldevices.CreateLogicalDevice(physicaldevice);
	swapchain.CreateSwapChain(windowsurface, physicaldevice, window, logicaldevices);
	swapchain.CreateImageView(logicaldevices);
	pipeline.createGraphicsPipeline(logicaldevices, swapchain);
    ComandPool.createCommandPool(logicaldevices,physicaldevice); 
    ComandPool.createCommandBuffer(logicaldevices); 
    render.createSyncObjects(logicaldevices);
};

void Engine::MainLoopEngine() {
	//window.ActualizarVentanas(logicaldevices, ComandPool, swapchain, pipeline, render);

    infoDraw info = {logicaldevices, ComandPool, swapchain, pipeline, render, windowsurface, physicaldevice, window};

        while (!glfwWindowShouldClose(window.GetWindows(0))){
            glfwSetKeyCallback(window.GetWindows(0), GLFW_KeyCallback);
            glfwPollEvents();

            //std::cout<<"Iniciando dibujo"<<std::endl;
            StartDrawFrame(info);
            
        }
        vkDeviceWaitIdle(logicaldevices.GetLogicalDevice());

};


void Engine::CleanEngine() {

    swapchain.destroySwapchain(logicaldevices);

	render.destroyFences(logicaldevices);
    ComandPool.destroyCommandPool(logicaldevices);



    vkDestroyPipeline(logicaldevices.GetLogicalDevice(), pipeline.getGrapicsPipeline(), nullptr);
    vkDestroyPipelineLayout(logicaldevices.GetLogicalDevice(), pipeline.getGrapicsPipelineLayout(), nullptr);
    vkDestroyShaderModule(logicaldevices.GetLogicalDevice(), pipeline.getShaderModule(), nullptr);



    vkDestroyDevice(logicaldevices.GetLogicalDevice(), nullptr);
    vkDestroySurfaceKHR(instance.GetInstance(), windowsurface.getSurface(), nullptr);
    
	instance.DestroyInstance();
	window.DesargarGLFW();

};


