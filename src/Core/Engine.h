#pragma once
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "Input/InputSystem.h"
#include "../Editor/EditorLayer.h"
#include "../Assets/Importer/AssimpImporter.h"
#include <GLFWWindow.h>

class Engine {
public:
	void runEngine();
	void CleanEngine();
private:
	void InitEngine();
	void MainLoopEngine();

	void EventManager();
	GLFWWindow window;
	InputSystem inputSystem;
	AssimpModelLoader assimploader;

	LoaderAssets loader;
	VulkanRHI VulkanAPI;
	ObjectInstance mesh;
	CameraView camera;
	EditorLayer layersUI;
};



