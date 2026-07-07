
#include "../Core/VulkanContext.h"
#include <iostream>


class VulkanWindow {

	public:
        VulkanWindow(VulkanContext& context)
            : m_Context(context)
        {
        }

		void InitWindowsSistem();
		void DestroyWindowsSistem();
		void CrearVentana(const char* name);
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        VulkanContext getContext() {return m_Context;};

		
	private:
		void PersonalizarVentana(VulkanContext context);

		void CargarGLFW();
		void DesargarGLFW();


        VulkanContext& m_Context;
};