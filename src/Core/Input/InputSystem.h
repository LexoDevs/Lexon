#pragma once

//#include <vulkan/vulkan.h>
//#include "../../RHI/Vulkan/VulkanRHI.h"
//#include "../../Renderer/Camera.h"
//#include "../../Assets/Loaders/LoaderAssets.h"

//static bool keys[GLFW_KEY_LAST + 1]{};
#include <InputCodes.h>
#include <array>

class InputSystem
{
public:

    void SetKeyState( KeyCode key, bool pressed );

    bool IsKeyPressed( KeyCode key ) const;

    void Selector(KeyCode &keysactives);
private:

    std::array<bool,static_cast<size_t>(KeyCode::Count)> m_Keys{};
};

/*
static void InputActionSelector(VulkanRHI& vulkanAPI, int input, int action) {
        CameraView::front;
        CameraView::position;
        CameraView::speed;
        CameraView::up;
        float velocity = CameraView::speed;

    if ((input == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) 
    {
        //glfwSetWindowShouldClose(vulkanAPI.GetVulkanWindow().getContext().GLFWwindow, GLFW_TRUE);
    }

    if (keys[GLFW_KEY_W]){
        //CameraView::position = CameraView::position + (CameraView::front*velocity);
        ObjectInstance::TranslateModel((CameraView::front*velocity));
    }

    if (keys[GLFW_KEY_S]){
        //CameraView::position = CameraView::position - (CameraView::front*velocity);

        ObjectInstance::TranslateModel(-(CameraView::front*velocity));

       //glm::vec3 dirUnit = VectorFrontalCamara(CameraView::dir);
        //ObjectInstance::TranslateModel(CameraView::center-dirUnit);
    }
        
    if (keys[GLFW_KEY_A]){

        glm::vec3 right = glm::normalize(glm::cross(CameraView::front, CameraView::up));

        ObjectInstance::TranslateModel(-(right*velocity));

    }

    if (keys[GLFW_KEY_D]){
        glm::vec3 right = glm::normalize(glm::cross(CameraView::front, CameraView::up));

        ObjectInstance::TranslateModel((right*velocity));

        
    }

    if (keys[GLFW_KEY_Q]){
        ObjectInstance::TranslateModel(-(CameraView::up*velocity));

    }

    if (keys[GLFW_KEY_E]){

        ObjectInstance::TranslateModel((CameraView::up*velocity));

    }

    
    if(keys[GLFW_KEY_LEFT]){
        ObjectInstance::RotateModel(-1.0f, 'y');

    }

    if(keys[GLFW_KEY_RIGHT]){
        ObjectInstance::RotateModel(1.0f, 'y');

    }
        if(keys[GLFW_KEY_UP]){
        ObjectInstance::RotateModel(-1.0f, 'x');

    }

    if(keys[GLFW_KEY_DOWN]){
        ObjectInstance::RotateModel(1.0f, 'x');

    }




    if ((input == GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_PRESS))
    {
        std::cout << "Click derecho detectado!" << std::endl;
    }

    if ((input == GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_RELEASE))
    {
            std::cout << "Click derecho soltado!" << std::endl;
    }

    if ((input == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS))
    {
        std::cout << "Click izquierdo detectado!" << std::endl;

    }

};


static void GLFW_KeyBoardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    VulkanRHI* vulkanAPI = static_cast<VulkanRHI*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;

    InputActionSelector(*vulkanAPI, key, action);

}

static void GLFW_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    VulkanRHI* vulkanAPI = static_cast<VulkanRHI*>(glfwGetWindowUserPointer(window));
    if (vulkanAPI)
    InputActionSelector(*vulkanAPI, button, action); 
}*/

