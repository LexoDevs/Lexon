#include "Camera.h"



void CameraView::SetCameraView(
    UniformBufferObject& ubo,
    float aspectRatio) const
{

    ubo.view = glm::lookAt(position, position + front, up);
ubo.proj = glm::perspective(
    glm::radians(60.0f),
    aspectRatio,
    1.0f,
    10000.0f
);

    ubo.proj[1][1] *= -1;

}
