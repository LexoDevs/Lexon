#pragma once
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "Input/InputSystem.h"
#include "../Editor/EditorLayer.h"
//Estructura personalizada para agrupar los datos de entrada en el dibujado de frames.

/*
struct infoDraw{

    Pool ComandPoolstr;
    Swapchain swapchainstr;
    GraphicsPipeline pipelinestr; 
    Render renderstr;
	VertexBuffer vertexbufferstr;
	Texture texturestr;
	DepthBuffer depthbufferstr;
	ObjectInstance meshstr;
	CameraView camerastr;
};*/


class Engine {
public:
	void runEngine();
	void StartDrawFrame();
	void CleanEngine();
private:
	void InitEngine();
	void MainLoopEngine();

	LoaderAssets loader;
	VulkanRHI VulkanAPI;
	ObjectInstance mesh;
	CameraView camera;
EditorLayer layersUI;
	/*
	Swapchain swapchain;
	GraphicsPipeline pipeline;
	Pool ComandPool;
	VertexBuffer vertexbuffer;
	Render render;
	Texture texture;
	DepthBuffer depthbuffer;
	uint32_t depthImage;
	ObjectInstance mesh;
*/
};



