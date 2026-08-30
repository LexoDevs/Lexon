
#include "VulkanSurface.h"

#include <cstdint> //uint32_t
#include <limits> //std::numeric_limits
#include <algorithm> //std::clamp
//############## Clase Surface Window ################//


VulkanSurface::VulkanSurface(){

};
// VkInstance instance, GLFWwindow* window 

VulkanSurface::~VulkanSurface(){
    DestroyVulkanSurface();
};


void VulkanSurface::CreateWindowSurface(void* window, VkInstance instance) {
    cp_Window = static_cast<GLFWwindow*>(window); 
    cp_Instance = instance;
	std::cout << "\033[1;36m[!] Creando superficie de ventana virtual...\033[0m\n";


    if (glfwCreateWindowSurface(cp_Instance, cp_Window, nullptr, &Surface) != 0) {
        throw std::runtime_error("failed to create window surface!");
    }

    std::cout << "\t\033[1;32mWindow Surface\033[0m:" << " alojada en \033[1;32m" << &Surface << "\033[0m\n\n";

};














void VulkanSurface::DestroyVulkanSurface(){
    if (Surface == VK_NULL_HANDLE)
    {
        return;
    }

    vkDestroySurfaceKHR(
        cp_Instance,
        Surface,
        nullptr
    );

    Surface = VK_NULL_HANDLE;
    
    std::cout<<"Superficie destruida"<<std::endl;

};











