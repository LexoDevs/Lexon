#pragma once

#include <cstdint> //uint32_t
#include <limits> //std::numeric_limits
#include <algorithm> //std::clamp

#include "Devices.h"
#include "../Renderer/Camera.h"
#include "../RHI/Vulkan/VulkanRHI.h"
#include "../Core/Input/InputSystem.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;



class Window {

	public:

		void InitWindowsSistem();
		void DestroyWindowsSistem();
		void CrearVentana(const char* name);
		void LimpiarVentanas();

		GLFWwindow* GetWindows(int i) { return GestorVentanas[i]; }
		std::vector<GLFWwindow*>& getListaVentanas() {return GestorVentanas;};
		
		static void framebufferResizeCallback(GLFWwindow* windowin, int width, int height);
		bool& getframebufferResized() { return framebufferResized;}
		
	private:
		void PersonalizarVentana(GLFWwindow* window);

		void CargarGLFW();
		void DesargarGLFW();

		std::vector<GLFWwindow*> GestorVentanas;

		bool framebufferResized = false;

};


class WindowSurface {

	public:
		void CreateWindowSurface(VulkanInstance& instance, Window window );
		
		VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(PhysicalDevice physicaldevice);
		VkPresentModeKHR GetSurfacePresentationsMode(PhysicalDevice physicaldevice);
		
		VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, Window window);
		
		std::vector<VkSurfaceFormatKHR> getSurfaceFormats(PhysicalDevice physicaldevice, uint32_t& pSurfaceFormatCount);
		
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats, uint32_t pSurfaceFormatCount);
		

		VkSurfaceKHR getSurface() { return surface; };

	private:

		VkSurfaceKHR surface;
};
