#pragma once


#include "../../RHI/Vulkan/VulkanRHI.h"
#include "../../Renderer/Camera.h"

static void InputActionSelector(GLFWwindow* window, int input, int action) {

    if ((input == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {

        glfwSetWindowShouldClose(window, GLFW_TRUE);

    }

	    if ((input == GLFW_KEY_W) && (action == GLFW_PRESS)) {
            std::cout<<"Hola perros"<<std::endl;

    }
};

static void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	InputActionSelector(window, key, action);

}
