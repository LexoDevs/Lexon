#include "RenderObject.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <limits>


glm::vec3 CalculateMeshPivot(
    const CpuMesh& mesh
)
{
    if (mesh.vertices.empty())
    {
        return glm::vec3(0.0f);
    }

    glm::vec3 minimum{
        std::numeric_limits<float>::max()
    };

    glm::vec3 maximum{
        std::numeric_limits<float>::lowest()
    };

    for (const CpuVertex& vertex : mesh.vertices)
    {
        minimum = glm::min(minimum, vertex.position);
        maximum = glm::max(maximum, vertex.position);
    }

    return (minimum + maximum) * 0.5f;
}


glm::mat4 Transform::CalculateMatrix(
    const glm::vec3& pivot
) const
{
    glm::mat4 result{1.0f};

    // Primero se desplaza el objeto.
    result = glm::translate(result, position);

    // Llevar el pivote al origen de la operación.
    result = glm::translate(result, pivot);

    result = glm::rotate(
        result,
        glm::radians(rotation.x),
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    result = glm::rotate(
        result,
        glm::radians(rotation.y),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    result = glm::rotate(
        result,
        glm::radians(rotation.z),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    result = glm::scale(result, scale);

    // Devolver el pivote a su posición original.
    result = glm::translate(result, -pivot);

    return result;
}

glm::mat4 RenderObject::CalculateWorldMatrix() const
{
    return importedTransform *
           transform.CalculateMatrix(localPivot);
}

namespace
{
    void BuildNodeObjects(
        const CpuNode& node,
        const CpuModel& model,
        const glm::mat4& parentTransform,
        std::vector<RenderObject>& output,
        uint32_t& nextId)
    {
        const glm::mat4 worldTransform =
            parentTransform * node.localTransform;

        for (uint32_t meshIndex : node.meshIndices)
        {
            if (meshIndex >= model.meshes.size())
            {
                continue;
            }

            const CpuMesh& mesh =
                model.meshes[meshIndex];

            RenderObject object{};

            object.id = nextId++;
            object.meshRangeIndex = meshIndex;
            object.materialIndex = mesh.materialIndex;
            object.importedTransform = worldTransform;
            object.localPivot = CalculateMeshPivot(mesh);

            if (!mesh.name.empty())
            {
                object.name = mesh.name;
            }
            else if (!node.name.empty())
            {
                object.name = node.name;
            }
            else
            {
                object.name =
                    "Object_" + std::to_string(object.id);
            }

            output.push_back(std::move(object));
        }

        for (const CpuNode& child : node.children)
        {
            BuildNodeObjects(
                child,
                model,
                worldTransform,
                output,
                nextId
            );
        }
    }
}

std::vector<RenderObject> BuildRenderObjects(
    const CpuModel& model
)
{
    std::vector<RenderObject> objects;

    // Puede haber más instancias que meshes si varios
    // nodos referencian una misma geometría.
    objects.reserve(model.meshes.size());

    uint32_t nextId = 1;

    BuildNodeObjects(
        model.rootNode,
        model,
        glm::mat4(1.0f),
        objects,
        nextId
    );

    return objects;
}