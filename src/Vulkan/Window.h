#pragma once

#include <cstdint> //uint32_t
#include <limits> //std::numeric_limits
#include <algorithm> //std::clamp

#include "Devices.h"
#include "Camera.h"
#include "../Runtime/RHI/Backends/Vulkan/VulkanRHI.h"

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

//############## Funcion Callback de Inputs ################//


static void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {

        glfwSetWindowShouldClose(window, GLFW_TRUE);

    }

	    if ((key == GLFW_KEY_W) && (action == GLFW_PRESS)) {


    }

}

class Window {

	public:

		void InitWindowsSistem();
		void DestroyWindowsSistem();
		void PersonalizarVentana(GLFWwindow* window);
		void CrearVentana(const char* name);
		void LimpiarVentanas();

		GLFWwindow* GetWindows(int i) { return GestorVentanas[i]; }
		std::vector<GLFWwindow*>& getListaVentanas() {return GestorVentanas;};
		
		static void framebufferResizeCallback(GLFWwindow* windowin, int width, int height);
		bool& getframebufferResized() { return framebufferResized;}
		
	private:
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
