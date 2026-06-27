#pragma once
#include "LoaderAssets.h"

class CameraView {

    public:

    void SetCameraView(UniformBufferObject& ubo, float aspectratio, float time,glm::vec3 dir);

};