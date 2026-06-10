#pragma once

#include <cstdint> //uint32_t
#include <limits> //std::numeric_limits
#include <algorithm> //std::clamp

#include "Devices.h"


constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

//############## Funcion Callback de Inputs ################//

static void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {

        glfwSetWindowShouldClose(window, GLFW_TRUE);

    }
}

class Window {

	public:

		void InitWindowsSistem();
		void CargarGLFW();
		void DesargarGLFW();
		static void framebufferResizeCallback(GLFWwindow* windowin, int width, int height);

		void CrearVentana(const char* name);

		//void ActualizarVentanas(LogicalDevice logicaldevices, Pool ComandPool, Swapchain swapchain, GraphicsPipeline pipeline, Render render);
		void LimpiarVentanas();
		GLFWwindow* GetWindows(int i) { return GestorVentanas[i]; }
		std::vector<GLFWwindow*>& getListaVentanas() {return GestorVentanas;};

		bool& getframebufferResized() { return framebufferResized;}
		bool framebufferResized = false;

		
	private:
		std::vector<GLFWwindow*> GestorVentanas;

};


class WindowSurface {

	public:
		void CreateWindowSurface(VulkanInstance instance, Window window );
		VkSurfaceKHR getSurface() { return surface; };
		VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(PhysicalDevice physicaldevice);
		VkPresentModeKHR GetSurfacePresentationsMode(PhysicalDevice physicaldevice);
		VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR surfaceCapabilities, Window window);
		std::vector<VkSurfaceFormatKHR> getSurfaceFormats(PhysicalDevice physicaldevice, uint32_t& pSurfaceFormatCount);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> availableFormats, uint32_t pSurfaceFormatCount);
		
	private:

		VkSurfaceKHR surface;
};
