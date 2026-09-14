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


bool ViewportPicker::IntersectsTriangle(
    const Ray& ray,
    const glm::vec3& vertex0,
    const glm::vec3& vertex1,
    const glm::vec3& vertex2,
    float& hitDistance)
{
    constexpr float epsilon = 0.000001f;

    const glm::vec3 edge1 =
        vertex1 - vertex0;

    const glm::vec3 edge2 =
        vertex2 - vertex0;

    const glm::vec3 perpendicular =
        glm::cross(
            ray.direction,
            edge2
        );

    const float determinant =
        glm::dot(
            edge1,
            perpendicular
        );

    // El rayo es paralelo al triángulo o
    // el triángulo no tiene superficie.
    if (std::abs(determinant) < epsilon)
    {
        return false;
    }

    const float inverseDeterminant =
        1.0f / determinant;

    const glm::vec3 distanceFromVertex =
        ray.origin - vertex0;

    const float barycentricU =
        glm::dot(
            distanceFromVertex,
            perpendicular
        ) * inverseDeterminant;

    if (barycentricU < 0.0f ||
        barycentricU > 1.0f)
    {
        return false;
    }

    const glm::vec3 crossProduct =
        glm::cross(
            distanceFromVertex,
            edge1
        );

    const float barycentricV =
        glm::dot(
            ray.direction,
            crossProduct
        ) * inverseDeterminant;

    if (barycentricV < 0.0f ||
        barycentricU + barycentricV > 1.0f)
    {
        return false;
    }

    const float distance =
        glm::dot(
            edge2,
            crossProduct
        ) * inverseDeterminant;

    // Evita seleccionar triángulos situados detrás
    // de la cámara o exactamente en su origen.
    if (distance <= epsilon)
    {
        return false;
    }

    hitDistance = distance;
    return true;
}

std::optional<uint32_t> ViewportPicker::PickObject(
    double mouseX,
    double mouseY,
    uint32_t viewportWidth,
    uint32_t viewportHeight,
    const CameraView& camera,
    const CpuModel& model,
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

        if (object.meshRangeIndex >=
            model.meshes.size())
        {
            continue;
        }

        const CpuMesh& mesh =
            model.meshes[object.meshRangeIndex];

        if (mesh.vertices.empty() ||
            mesh.indices.size() < 3)
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
            glm::vec4(
                worldRay.origin,
                1.0f
            )
        );

        localRay.direction = glm::vec3(
            inverseWorldMatrix *
            glm::vec4(
                worldRay.direction,
                0.0f
            )
        );

        // Primera fase: comprobación rápida contra AABB.
        float boundingBoxDistance = 0.0f;

        if (!IntersectsBoundingBox(
                localRay,
                object.localBounds,
                boundingBoxDistance))
        {
            continue;
        }

        // Segunda fase: comprobar la geometría real.
        float closestMeshDistance =
            std::numeric_limits<float>::max();

        for (std::size_t index = 0;
             index + 2 < mesh.indices.size();
             index += 3)
        {
            const uint32_t index0 =
                mesh.indices[index];

            const uint32_t index1 =
                mesh.indices[index + 1];

            const uint32_t index2 =
                mesh.indices[index + 2];

            // Protección frente a índices incorrectos.
            if (index0 >= mesh.vertices.size() ||
                index1 >= mesh.vertices.size() ||
                index2 >= mesh.vertices.size())
            {
                continue;
            }

            const glm::vec3& vertex0 =
                mesh.vertices[index0].position;

            const glm::vec3& vertex1 =
                mesh.vertices[index1].position;

            const glm::vec3& vertex2 =
                mesh.vertices[index2].position;

            float triangleDistance = 0.0f;

            if (IntersectsTriangle(
                    localRay,
                    vertex0,
                    vertex1,
                    vertex2,
                    triangleDistance) &&
                triangleDistance <
                    closestMeshDistance)
            {
                closestMeshDistance =
                    triangleDistance;
            }
        }

        // La AABB fue atravesada, pero ninguno de
        // los triángulos reales fue alcanzado.
        if (closestMeshDistance ==
            std::numeric_limits<float>::max())
        {
            continue;
        }

        if (closestMeshDistance < closestDistance)
        {
            closestDistance =
                closestMeshDistance;

            closestObjectId =
                object.id;
        }
    }

    return closestObjectId;
}