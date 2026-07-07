#include "Camera.h"


void CameraView::SetCameraView(UniformBufferObject& ubo, float aspectratio, float time){

    ubo.view = glm::lookAt(dir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), aspectratio, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

};

