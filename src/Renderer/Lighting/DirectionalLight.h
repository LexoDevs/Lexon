#pragma once

#include <glm/glm.hpp>

#include <cmath>

struct DirectionalLight
{
    // Dirección en la que viajan los rayos solares.
    glm::vec3 direction = {-0.0f, -1.0f, 1.0f};
    
    float intensity = 100.0f;

    glm::vec3 color{
        1.0f,
        0.55f,
        0.20f
    };

    float ambientIntensity = 0.15f;

    [[nodiscard]]
    glm::vec3 GetNormalizedDirection() const
    {
        const float lengthSquared =
            glm::dot(direction, direction);

        if (lengthSquared < 0.000001f)
        {
            // Evita normalizar un vector cero.
            return glm::vec3(
                0.0f,
                -1.0f,
                0.0f
            );
        }

        return direction /
               std::sqrt(lengthSquared);
    }
};