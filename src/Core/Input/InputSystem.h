#pragma once

#include <vulkan/vulkan.h>
#include "../../RHI/Vulkan/VulkanRHI.h"
#include "../../Renderer/Camera.h"

static void InputActionSelector(VulkanRHI& vulkanAPI, int input, int action) {


    if ((input == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(vulkanAPI.GetVulkanWindow().getContext().GLFWwindow, GLFW_TRUE);
    }


	if ((input == GLFW_KEY_W) && (action == GLFW_PRESS)) {
                std::cout<<"Hola perros"<<std::endl;
                CameraView newcam;
                UniformBufferObject uniform = vulkanAPI.GetVulkanContext().UBO;
                
               // vulkanAPI.GetUniformBuffer().updateUniformBuffer();
    }
};

static void GLFW_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    VulkanRHI* vulkanAPI = static_cast<VulkanRHI*>(glfwGetWindowUserPointer(window));

    InputActionSelector(*vulkanAPI, key, action);

}
