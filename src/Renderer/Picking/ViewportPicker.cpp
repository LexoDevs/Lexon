#include "ViewportPicker.h"

#include <glm/gtc/matrix_inverse.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

Ray ViewportPicker::BuildRay(
    double mouseX,
    double mouseY,
    uint32_t viewportWidth,
    uint32_t viewportHeight,
    const CameraView& camera)
{
    Ray ray{};
    ray.origin = camera.position;

    if (viewportWidth == 0 ||
        viewportHeight == 0)
    {
        ray.direction =
            glm::normalize(camera.front);

        return ray;
    }

    const float width =
        static_cast<float>(viewportWidth);

    const float height =
        static_cast<float>(viewportHeight);

    const float normalizedX =
        (2.0f * static_cast<float>(mouseX) / width)
        - 1.0f;

    const float normalizedY =
        1.0f
        - (2.0f * static_cast<float>(mouseY) / height);

    const float aspectRatio =
        width / height;

    const float tangent =
        std::tan(
            glm::radians(camera.fieldOfView) * 0.5f
        );

    const glm::vec3 forward =
        glm::normalize(camera.front);

    const glm::vec3 right =
        glm::normalize(
            glm::cross(forward, camera.up)
        );

    const glm::vec3 correctedUp =
        glm::normalize(
            glm::cross(right, forward)
        );

    ray.direction = glm::normalize(
        forward +
        right * normalizedX * aspectRatio * tangent +
        correctedUp * normalizedY * tangent
    );

    return ray;
}

bool ViewportPicker::IntersectsBoundingBox(
    const Ray& ray,
    const BoundingBox& bounds,
    float& hitDistance)
{
    float minimumDistance = 0.0f;

    float maximumDistance =
        std::numeric_limits<float>::max();

    constexpr float epsilon = 0.000001f;

    for (int axis = 0; axis < 3; ++axis)
    {
        const float origin = ray.origin[axis];
        const float direction = ray.direction[axis];

        const float boundsMinimum =
            bounds.minimum[axis];

        const float boundsMaximum =
            bounds.maximum[axis];

        if (std::abs(direction) < epsilon)
        {
            if (origin < boundsMinimum ||
                origin > boundsMaximum)
            {
                return false;
            }

            continue;
        }

        const float inverseDirection =
            1.0f / direction;

        float firstDistance =
            (boundsMinimum - origin) *
            inverseDirection;

        float secondDistance =
            (boundsMaximum - origin) *
            inverseDirection;

        if (firstDistance > secondDistance)
        {
            std::swap(
                firstDistance,
                secondDistance
            );
        }

        minimumDistance = std::max(
            minimumDistance,
            firstDistance
        );

        maximumDistance = std::min(
            maximumDistance,
            secondDistance
        );

        if (minimumDistance > maximumDistance)
        {
            return false;
        }
    }

    hitDistance = minimumDistance;
    return true;
}

std::optional<uint32_t> ViewportPicker::PickObject(
    double mouseX,
    double mouseY,
    uint32_t viewportWidth,
    uint32_t viewportHeight,
    const CameraView& camera,
    const std::vector<RenderObject>& objects)
{
    const Ray worldRay = BuildRay(
        mouseX,
        mouseY,
        viewportWidth,
        viewportHeight,
        camera
    );

    std::optional<uint32_t> closestObjectId;

    float closestDistance =
        std::numeric_limits<float>::max();

    for (const RenderObject& object : objects)
    {
        if (!object.visible)
        {
            continue;
        }

        const glm::mat4 worldMatrix =
            object.CalculateWorldMatrix();

        const glm::mat4 inverseWorldMatrix =
            glm::inverse(worldMatrix);

        Ray localRay{};

        localRay.origin = glm::vec3(
            inverseWorldMatrix *
            glm::vec4(worldRay.origin, 1.0f)
        );

        localRay.direction = glm::vec3(
            inverseWorldMatrix *
            glm::vec4(worldRay.direction, 0.0f)
        );

        float hitDistance = 0.0f;

        if (IntersectsBoundingBox(
                localRay,
                object.localBounds,
                hitDistance) &&
            hitDistance < closestDistance)
        {
            closestDistance = hitDistance;
            closestObjectId = object.id;
        }
    }

    return closestObjectId;
}