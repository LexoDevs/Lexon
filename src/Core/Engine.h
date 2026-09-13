#pragma once
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "Input/InputSystem.h"
#include "../Editor/EditorLayer.h"
#include "../Assets/Importer/AssimpImporter.h"
#include "../Assets/Textures/TextureImporter.h"
#include <GLFWWindow.h>
#include "../Renderer/RenderObject.h"
class Engine {
	public:

		void runEngine();
		void CleanEngine();

	private:

		void InitEngine();
		void MainLoopEngine();
		void EventManager(float deltaTime);
		void LoadUIPanels();

		GLFWWindow window;
		InputSystem inputSystem;


		AssimpModelLoader assimploader;
		TextureImporter textureimport;

		LoaderAssets loader;
		VulkanRHI VulkanAPI;
		EditorLayer layersUI;

		CpuModel model;
		std::vector<RenderObject> renderObjects;
		CameraView camera;

};



