#include "Camera.h"

glm::vec3 CameraView::position = glm::vec3(0.0f, 0.3f, 0.0f);
glm::vec3 CameraView::front    = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 CameraView::up       = glm::vec3(0.0f, 1.0f, 0.0f);   // Y es arriba

float CameraView::speed = 0.8f;

void CameraView::SetCameraView(UniformBufferObject& ubo, float aspectratio, float time){

    ubo.view = glm::lookAt(front, position, up);
    ubo.proj = glm::perspective(glm::radians(45.0f), aspectratio, 0.1f, 2000.0f);
    ubo.proj[1][1] *= -1;

};

