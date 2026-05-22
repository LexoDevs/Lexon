#pragma once


#include "Vulkan/Render.h"

class Engine {
public:
	void runEngine();

	void StartDrawFrame(LogicalDevice logicaldevices, Pool ComandPool, Swapchain swapchain, GraphicsPipeline pipeline, Render render);

private:

	Window window;
	VulkanInstance instance;
	PhysicalDevice physicaldevice;
	LogicalDevice logicaldevices;
	WindowSurface windowsurface;
	Swapchain swapchain;
	GraphicsPipeline pipeline;
	Pool ComandPool;
	Render render;

	
	void InitEngine();
	void MainLoopEngine();
	void CleanEngine();
};


static Engine masterengine = {};

