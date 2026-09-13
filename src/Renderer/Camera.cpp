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

void CameraView::ProcessMouseMovement(
    float deltaX,
    float deltaY)
{
    yaw += deltaX * mouseSensitivity;

    // GLFW incrementa Y hacia abajo.
    pitch -= deltaY * mouseSensitivity;

    pitch = glm::clamp(
        pitch,
        -89.0f,
        89.0f
    );

    UpdateDirection();
}

void CameraView::UpdateDirection()
{
    const float yawRadians =
        glm::radians(yaw);

    const float pitchRadians =
        glm::radians(pitch);

    glm::vec3 direction{};

    direction.x =
        std::cos(yawRadians) *
        std::cos(pitchRadians);

    direction.y =
        std::sin(pitchRadians);

    direction.z =
        std::sin(yawRadians) *
        std::cos(pitchRadians);

    front = glm::normalize(direction);
}