#pragma once
#include "../Assets/Loaders/LoaderAssets.h"

class CameraView {

    public:
    glm::vec3 dir = glm::vec3(2.0f, 2.0f, 2.0f);
;

    void SetCameraView(UniformBufferObject& ubo, float aspectratio, float time);

};