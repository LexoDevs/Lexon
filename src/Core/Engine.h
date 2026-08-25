#pragma once
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "Input/InputSystem.h"
#include "../Editor/EditorLayer.h"

class Engine {
public:
	void runEngine();
	void CleanEngine();
private:
	void InitEngine();
	void MainLoopEngine();

	GLFWWindow window;

	LoaderAssets loader;
	VulkanRHI VulkanAPI;
	ObjectInstance mesh;
	CameraView camera;
	EditorLayer layersUI;

};



