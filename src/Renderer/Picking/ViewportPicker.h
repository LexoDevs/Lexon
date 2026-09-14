#pragma once

#include "../Camera.h"
#include "../RenderObject.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <optional>
#include <vector>

struct Ray
{
    glm::vec3 origin{0.0f};
    glm::vec3 direction{0.0f, 0.0f, -1.0f};
};

class ViewportPicker
{
public:
    static Ray BuildRay(
        double mouseX,
        double mouseY,
        uint32_t viewportWidth,
        uint32_t viewportHeight,
        const CameraView& camera
    );

static std::optional<uint32_t> PickObject(
    double mouseX,
    double mouseY,
    uint32_t viewportWidth,
    uint32_t viewportHeight,
    const CameraView& camera,
    const CpuModel& model,
    const std::vector<RenderObject>& objects
);

private:
    static bool IntersectsBoundingBox(
        const Ray& ray,
        const BoundingBox& bounds,
        float& hitDistance
    );

    static bool IntersectsTriangle(
    const Ray& ray,
    const glm::vec3& vertex0,
    const glm::vec3& vertex1,
    const glm::vec3& vertex2,
    float& hitDistance
);
};