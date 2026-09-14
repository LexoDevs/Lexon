#pragma once
#include "../Assets/Loaders/LoaderAssets.h"

class CameraView {

    public:
glm::vec3 position = glm::vec3(600.0f, 300.0f, 0.0f);
glm::vec3 front    = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);   // Y es arriba/ Y es arriba

    float yaw   = 180.0f;
    float pitch = 0.0f;

        float movementSpeed = 500.0f;
        float mouseSensitivity = 0.4f;




float fieldOfView = 60.0f;
float nearPlane = 1.0f;
float farPlane = 10000.0f;


void ProcessMouseMovement(float deltaX,float deltaY);
    void SetCameraView(UniformBufferObject& ubo, float aspectratio) const;
glm::vec3& GerPos() {return position;};


void MoverAdelante(float amount) {position += front * amount;};
void MoverIzquierda(float amount) {
    const glm::vec3 right = glm::normalize(glm::cross(front, up));
    position += right * amount;};

void MoverArriba(float amount)
{
    position += up * amount;
}



private:
    void UpdateDirection();


};