#pragma once
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "Input/InputSystem.h"
#include "../Editor/EditorLayer.h"
#include "../Assets/Importer/AssimpImporter.h"
#include "../Assets/Textures/TextureImporter.h"
#include <GLFWWindow.h>

class Engine {
	public:

		void runEngine();
		void CleanEngine();

	private:

		void InitEngine();
		void MainLoopEngine();
		void EventManager();
		void LoadUIPanels();

		GLFWWindow window;
		InputSystem inputSystem;

		EditorLayer layersUI;

		AssimpModelLoader assimploader;
		TextureImporter textureimport;

		LoaderAssets loader;
		VulkanRHI VulkanAPI;
		ObjectInstance mesh;
		CameraView camera;

		CpuModel model;
};



