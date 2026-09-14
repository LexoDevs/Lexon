#pragma once
#include "../Assets/Loaders/LoaderAssets.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "Input/InputSystem.h"
#include "../Editor/EditorLayer.h"
#include "../Assets/Importer/AssimpImporter.h"
#include "../Assets/Textures/TextureImporter.h"
#include <GLFWWindow.h>
#include "../Renderer/RenderObject.h"
#include "../Renderer/RenderSettings.h"
#include "../Renderer/Lighting/DirectionalLight.h"
#include "../Editor/EditorSelection.h"
#include "../Renderer/Picking/ViewportPicker.h"


class Engine {
	public:

		void runEngine();
		void CleanEngine();

	private:

		void InitEngine();
		void MainLoopEngine();
		void EventManager(float deltaTime);
		void HandleViewportSelection(const EditorInputCapture& capture);



		void LoadUIPanels();

		GLFWWindow window;
		InputSystem inputSystem;


		AssimpModelLoader assimploader;
		TextureImporter textureimport;

		LoaderAssets loader;
		VulkanRHI VulkanAPI;
		EditorLayer layersUI;
		EditorSelection editorSelection;

		CpuModel model;
		std::vector<RenderObject> renderObjects;
		CameraView camera;
		RenderSettings renderSettings;
		DirectionalLight sunLight;
};



