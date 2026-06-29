#pragma once
#include "../Vulkan/Render.h"
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
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
	VertexBuffer vertexbufferstr;
	Texture texturestr;
	DepthBuffer depthbufferstr;
	ObjectInstance meshstr;
	CameraView camerastr;
};


class Engine {
public:
	void runEngine();
	void StartDrawFrame(infoDraw& structureDraw);

private:
	void InitEngine();
	void MainLoopEngine();
	void CleanEngine();

	VulkanRHI VulkanAPI;

	Window window;
	PhysicalDevice physicaldevice;
	LogicalDevice logicaldevices;
	WindowSurface windowsurface;
	Swapchain swapchain;
	GraphicsPipeline pipeline;
	Pool ComandPool;
	VertexBuffer vertexbuffer;
	Render render;
	Texture texture;
	DepthBuffer depthbuffer;
uint32_t depthImage;
	ObjectInstance mesh;
CameraView camera;


	LoaderAssets loader;
};



