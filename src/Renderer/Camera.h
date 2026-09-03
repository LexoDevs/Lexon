#pragma once
#include "../Assets/Loaders/LoaderAssets.h"

class CameraView {

    public:
glm::vec3 position = glm::vec3(600.0f, 300.0f, 0.0f);
glm::vec3 front    = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);   // Y es arriba/ Y es arriba

        float speed = 0.8f;
    
    void SetCameraView(UniformBufferObject& ubo, float aspectratio) const;
glm::vec3& GerPos() {return position;};
void alante() {position = position + glm::vec3(1.0f, 0.0f, 0.0f);};

};