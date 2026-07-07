#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "VulkanWindow.h"
#include "../../../Core/WindowSystem/WindowContext.h"


#include "dwmapi.h"
//############## Clase Window ################//
 
void VulkanWindow::CargarGLFW() {
    
    glfwInit();

}

void VulkanWindow::DesargarGLFW() {

    glfwTerminate();

}

void VulkanWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<VulkanWindow*>(glfwGetWindowUserPointer(window));
    app->m_Context.framebufferResized = true;

}

void VulkanWindow::PersonalizarVentana(VulkanContext context){

    #ifdef _WIN32
    HWND hwnd = glfwGetWin32Window(m_Context.GLFWwindow);
    if (hwnd)
    {
        // ==================== COLOR DE TÍTULO Y BORDE ====================
        // Azul personalizado (puedes cambiar los valores RGB)
        COLORREF captionColor = RGB(0, 100, 255);     // Azul bonito
        COLORREF borderColor  = RGB(0, 80, 220);      // Azul un poco más oscuro para el borde

        // Color de la barra de título
        DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR, 
                             &captionColor, sizeof(captionColor));

        // Color del borde de la ventana
        DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, 
                             &borderColor, sizeof(borderColor));

        // Opcional: Color del texto del título (blanco para que se vea bien)
        COLORREF titleTextColor = RGB(255, 255, 255);
        DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR, 
                             &titleTextColor, sizeof(titleTextColor));

        // Esquinas redondeadas (Windows 11)
        DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_ROUND;
        DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, 
                             &preference, sizeof(preference));
    }
    #endif

};

void VulkanWindow::CrearVentana(const char* name)
{
    WindowProperties prop;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    std::cout << "\033[1;36m[!] Creando ventana...\033[0m\n";
    
    m_Context.GLFWwindow = glfwCreateWindow(prop.width, prop.height, name, nullptr, nullptr);
    if (!m_Context.GLFWwindow)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }


    PersonalizarVentana(m_Context);

    glfwSetFramebufferSizeCallback(m_Context.GLFWwindow, framebufferResizeCallback);

    std::cout << "\t\033[1;32m" << name << "\033[0m guardada en \033[1;32m" 
              << m_Context.GLFWwindow << "\033[0m\n\n";

    glfwSetWindowUserPointer(m_Context.GLFWwindow, this);
}


void VulkanWindow::InitWindowsSistem() {
    //Cargado de la libreria GLFW para gestionar ventanas
    CargarGLFW();

    //Creacion de una ventana
    CrearVentana("Cargando...");

    
};

void VulkanWindow::DestroyWindowsSistem(){

    DesargarGLFW();

};

