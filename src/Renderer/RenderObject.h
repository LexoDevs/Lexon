#pragma once

#include "Assets/Importer/CPUModel.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <vector>

struct Transform
{
    // Desplazamiento respecto de la posición importada.
    glm::vec3 position{0.0f};

    // Grados, no radianes.
    glm::vec3 rotation{0.0f};

    glm::vec3 scale{1.0f};

    [[nodiscard]]
    glm::mat4 CalculateMatrix(
        const glm::vec3& pivot
    ) const;
};

struct RenderObject
{
    uint32_t id = 0;
    std::string name;

    // Índice dentro de IndexBuffer::meshRanges.
    uint32_t meshRangeIndex = 0;

    uint32_t materialIndex = 0;

    Transform transform;

    // Transformación acumulada de la jerarquía Assimp.
    glm::mat4 importedTransform{1.0f};

    // Centro local usado para rotar/escalar la malla.
    glm::vec3 localPivot{0.0f};

    bool visible = true;

    [[nodiscard]]
    glm::mat4 CalculateWorldMatrix() const;
};

// Datos enviados antes de cada draw.
struct ObjectPushConstants
{
    glm::mat4 model{1.0f};
    glm::mat4 normalMatrix{1.0f};
};

static_assert(
    sizeof(ObjectPushConstants) == 128,
    "ObjectPushConstants debe ocupar 128 bytes"
);


std::vector<RenderObject> BuildRenderObjects(
    const CpuModel& model
);