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
        structureDraw.windowstr,
        structureDraw.vertexbufferstr,
        structureDraw.texturestr,
        structureDraw.depthbufferstr,  
        structureDraw.depthbufferstr.getClearImageView(),
        structureDraw.depthbufferstr.getdepthImage()

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
    pipeline.CreateDescriptorSetLayout(logicaldevices);

	pipeline.createGraphicsPipeline(logicaldevices, swapchain, depthbuffer.findDepthFormat(physicaldevice));

    ComandPool.createCommandPool(logicaldevices,physicaldevice); 
    depthbuffer.createDepthResources(physicaldevice, texture, logicaldevices, vertexbuffer, swapchain);


    texture.createTextureImage(vertexbuffer,logicaldevices, physicaldevice, ComandPool);
    texture.createTextureImageView(logicaldevices, VK_IMAGE_ASPECT_DEPTH_BIT);
    texture.createTextureSampler(logicaldevices, physicaldevice);

    vertexbuffer.createVertexBuffer(logicaldevices,physicaldevice,ComandPool, texture.getTextureImage());
    vertexbuffer.createIndexBuffer(logicaldevices,physicaldevice,ComandPool, texture.getTextureImage());

    vertexbuffer.createUniformBuffer(logicaldevices,physicaldevice);
    ComandPool.createDescriptorPool(logicaldevices);
    texture.createDescriptorSets(logicaldevices, pipeline, ComandPool, vertexbuffer);

    vertexbuffer.createCommandBuffer(logicaldevices, ComandPool); 

    render.createSyncObjects(logicaldevices);
};

void Engine::MainLoopEngine() {

    infoDraw info = {logicaldevices, ComandPool, swapchain, pipeline, render, windowsurface, physicaldevice, window, vertexbuffer, texture, depthbuffer };

    double previousTime = glfwGetTime();
    int frameCount = 0;
    double fps = 0.0;

    while (!glfwWindowShouldClose(window.GetWindows(0))){

        glfwSetKeyCallback(window.GetWindows(0), GLFW_KeyCallback);
        glfwPollEvents();

        StartDrawFrame(info);        
        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0) {
            fps = frameCount / (currentTime - previousTime);
            previousTime = currentTime;
            frameCount = 0;
            //Cambiar titulo añadiendo los FPS
            std::string title = "Vulkan Engine - FPS: " + std::to_string(static_cast<int>(fps));
            glfwSetWindowTitle(window.GetWindows(0), title.c_str());
        }

        // Mostrar FPS en consola 
        static double lastPrintTime = 0.0;
        if (currentTime - lastPrintTime >= 0.2) {  // Imprimir cada 2 segundos
            std::cout << "\rFPS: " << std::fixed << std::setprecision(1) << fps << "    " << std::flush;
            lastPrintTime = currentTime;
        }
    }
        std::cout<<std::endl;

    vkDeviceWaitIdle(logicaldevices.GetLogicalDevice());

};


void Engine::CleanEngine() {
    depthbuffer.destroyDepthResources(logicaldevices);
    swapchain.destroySwapchain(logicaldevices);
    texture.destroyImageTextureView(logicaldevices);
    texture.destroyImageTexture(logicaldevices);

    pipeline.DestroyDescriptorSetLayout(logicaldevices);

    vertexbuffer.destroyVertexBuffer(logicaldevices);

    vertexbuffer.destroyUniformBuffer(logicaldevices);

	render.destroyFences(logicaldevices);
    ComandPool.destroyCommandPool(logicaldevices);

    texture.destroyDescriptorSet(logicaldevices);

    vkDestroyPipeline(logicaldevices.GetLogicalDevice(), pipeline.getGrapicsPipeline(), nullptr);
    vkDestroyPipelineLayout(logicaldevices.GetLogicalDevice(), pipeline.getGrapicsPipelineLayout(), nullptr);
    vkDestroyShaderModule(logicaldevices.GetLogicalDevice(), pipeline.getShaderModule(), nullptr);

    vkDestroyDevice(logicaldevices.GetLogicalDevice(), nullptr);
    vkDestroySurfaceKHR(instance.GetInstance(), windowsurface.getSurface(), nullptr);
    
	instance.DestroyInstance();

	window.DestroyWindowsSistem();

};


