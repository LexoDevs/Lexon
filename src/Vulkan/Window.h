#pragma once

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp



#include "Devices.h"


constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

class Window {

public:
	void InitWindowsSistem();
	void CargarGLFW();
	void DesargarGLFW();
	void CrearVentana(const char* name);
	//void ActualizarVentanas(LogicalDevice logicaldevices, Pool ComandPool, Swapchain swapchain, GraphicsPipeline pipeline, Render render);
	void LimpiarVentanas();
	GLFWwindow* GetWindows(int i) { return GestorVentanas[0]; }

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
