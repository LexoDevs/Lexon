#pragma once
#include "../Assets/Loaders/LoaderAssets.h"

class CameraView {

    public:
    static glm::vec3 front;
    static glm::vec3 position;
    static glm::vec3 up;
    static float speed;
    
    void SetCameraView(UniformBufferObject& ubo, float aspectratio, float time);

};