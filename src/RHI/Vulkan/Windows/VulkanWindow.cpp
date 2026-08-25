#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "VulkanWindow.h"


#include "dwmapi.h"

void GLFWWindow::InitWindow(){
    glfwInit();
    CreateGLFWindow();
};

void GLFWWindow::DestroyWindow(){
    glfwTerminate();
};

bool GLFWWindow::ShouldClose() const
{
    return glfwWindowShouldClose(glfwwindow);
}

void GLFWWindow::PoolEvents() const
{
            glfwPollEvents();
}

double GLFWWindow::GetTime() const {
    return glfwGetTime();

};

void GLFWWindow::SetWindowTitle(std::string title) const {
        glfwSetWindowTitle(glfwwindow, title.c_str());

};

void GLFWWindow::SetKeyCallback() const {
    glfwSetKeyCallback(glfwwindow, GLFW_KeyBoardCallback);
};

void GLFWWindow::CreateGLFWindow(){

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    std::cout << "\033[1;36m[!] Creando ventana...\033[0m\n";
    
    glfwwindow = glfwCreateWindow(width, height, "Hola", nullptr, nullptr);

    if (!glfwwindow)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(glfwwindow, this);

    glfwSetFramebufferSizeCallback(glfwwindow, framebufferResizeCallback);

    std::cout << "\t\033[1;32m" << title << "\033[0m guardada en \033[1;32m" 
              << glfwwindow << "\033[0m\n\n";
    
    size_t size = sizeof(glfwwindow);
    double megabytes = static_cast<double>(size) / (1024.0 * 1024.0);
    std::cout << "Tamaño en bytes: " << size << std::endl;

    std::cout << "Tamaño en MB: " << megabytes << std::endl;

};

void GLFWWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto glfwWindow  = reinterpret_cast<GLFWWindow*>(
        glfwGetWindowUserPointer(window)
    );
    glfwWindow->width = width;
    glfwWindow->height = height;

    glfwWindow->framebufferResized = true;

}

