#pragma once
#include "Vulkan/Render.h"

//Estructura personalizada para agrupar los datos de entrada en el dibujado de frames.
struct infoDraw{
    LogicalDevice logicaldevicesstr;
    Pool ComandPoolstr;
    Swapchain swapchainstr;
    GraphicsPipeline pipelinestr; 
    Render renderstr;
	WindowSurface windowsurfacestr;
	PhysicalDevice physicaldevicestr;
	Window windowstr;
};


class Engine {
public:
	void runEngine();
	void StartDrawFrame(infoDraw& structureDraw);

private:
	void InitEngine();
	void MainLoopEngine();
	void CleanEngine();

	Window window;
	VulkanInstance instance;
	PhysicalDevice physicaldevice;
	LogicalDevice logicaldevices;
	WindowSurface windowsurface;
	Swapchain swapchain;
	GraphicsPipeline pipeline;
	Pool ComandPool;
	Render render;

};



