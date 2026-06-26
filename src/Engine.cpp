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
        structureDraw.depthbufferstr.getdepthImage(),
        structureDraw.meshstr
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

<<<<<<< HEAD
    mesh.AddObject(loader,0);
    mesh.AddObject(loader,1);
    mesh.AddObject(loader,2);

       std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    mesh.PrepareLevelObjects(vertices, indices);

    std::cout<<"Primer objeto inicializado"<<std::endl;
    LevelObjects[0].setposition({0.0f, 1.0f, 0.0f});
    LevelObjects[0].setrotation({0.0f, 0.0f, 0.0f});
    LevelObjects[0].setscale({1.0f, 1.0f, 1.0f});

    // Object 2 - Left
    std::cout<<"Segundo objeto inicializado"<<std::endl;
    LevelObjects[1].setposition({-2.0f, 0.0f, -1.0f});
    LevelObjects[1].setrotation({0.0f, 0.0f, 0.0f});
    LevelObjects[1].setscale({0.75f, 0.75f, 0.75f});

    // Object 3 - Right
    std::cout<<"Tercer objeto inicializado"<<std::endl;
    LevelObjects[2].setposition({2.0f, 0.0f, -1.0f});
    LevelObjects[2].setrotation({0.0f, 0.0f, 0.0f});
    LevelObjects[2].setscale({0.75f, 0.75f, 0.75f});



        
    std::cout<<"Vertices: "<<vertices.size()<<std::endl;

    vertexbuffer.createVertexBuffer(logicaldevices,physicaldevice,ComandPool, texture.getTextureImage(),vertices);
    vertexbuffer.createIndexBuffer(logicaldevices,physicaldevice,ComandPool, texture.getTextureImage(),indices);
//    mesh.PrepareLevelObjects();
    
    //mesh.setupGameObjects();

        std::cout<<"Posicion1: "<<LevelObjects[0].getposition().x<<" "<<LevelObjects[0].getposition().y<< " "<<LevelObjects[0].getposition().z<<std::endl;
        std::cout<<"Posicion2: "<<LevelObjects[1].getposition().x<<" "<<LevelObjects[1].getposition().y<< " "<<LevelObjects[1].getposition().z<<std::endl;
=======
    mesh.AddObject(loader);
    vertexbuffer.createVertexBuffer(logicaldevices,physicaldevice,ComandPool, texture.getTextureImage(),mesh);
    vertexbuffer.createIndexBuffer(logicaldevices,physicaldevice,ComandPool, texture.getTextureImage(),mesh);
>>>>>>> parent of 64c4e27 (Primer Game Object (Corregir))

    vertexbuffer.createUniformBuffer(logicaldevices,physicaldevice);
    ComandPool.createDescriptorPool(logicaldevices);
    texture.createDescriptorSets(logicaldevices, pipeline, ComandPool, vertexbuffer);

    vertexbuffer.createCommandBuffer(logicaldevices, ComandPool); 

    render.createSyncObjects(logicaldevices);
};

void Engine::MainLoopEngine() {

    infoDraw info = {logicaldevices, ComandPool, swapchain, pipeline, render, windowsurface, physicaldevice, window, vertexbuffer, texture, depthbuffer, mesh };

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


