#include "Engine.h"




void Engine::runEngine() {

	InitEngine();
	MainLoopEngine();
	CleanEngine();
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


    while (!glfwWindowShouldClose(window.GetWindows(0))){
        
        std::cout<<"en bucle" <<"\n";

        glfwPollEvents();
        StartDrawFrame(logicaldevices, ComandPool, swapchain, pipeline, render);

    }
    vkDeviceWaitIdle(logicaldevices.GetLogicalDevice());

        
};


void Engine::StartDrawFrame(LogicalDevice logicaldevices, Pool ComandPool, Swapchain swapchain, GraphicsPipeline pipeline, Render render){

            render.drawFrame(logicaldevices, ComandPool, swapchain, pipeline);

};

void Engine::CleanEngine() {
	
    render.destroyFences(logicaldevices);

    ComandPool.destroyCommandPool(logicaldevices);


        vkDestroyPipeline(logicaldevices.GetLogicalDevice(), pipeline.getGrapicsPipeline(), nullptr);
        vkDestroyPipelineLayout(logicaldevices.GetLogicalDevice(), pipeline.getGrapicsPipelineLayout(), nullptr);

        vkDestroyShaderModule(logicaldevices.GetLogicalDevice(), pipeline.getShaderModule(), nullptr);

        for (auto imageView : swapchain.getSwapchainImageView()) {
            vkDestroyImageView(logicaldevices.GetLogicalDevice(), imageView, nullptr);
        }
        vkDestroySwapchainKHR(logicaldevices.GetLogicalDevice(), swapchain.getSwapchain(), nullptr);
        vkDestroyDevice(logicaldevices.GetLogicalDevice(), nullptr);


    vkDestroySurfaceKHR(instance.GetInstance(), windowsurface.getSurface(), nullptr);


	instance.DestroyInstance();
	window.DesargarGLFW();

};


