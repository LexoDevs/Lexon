#include "Camera.h"




void CameraView::SetCameraView(UniformBufferObject& ubo, float aspectratio, float time){

    ubo.view = glm::lookAt(front, position, up);
    ubo.proj = glm::perspective(glm::radians(45.0f), aspectratio, 0.1f, 2000.0f);
    ubo.proj[1][1] *= -1;

};

