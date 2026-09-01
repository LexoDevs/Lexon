#pragma once

#include "CPUModel.h"

#include <filesystem>
#include <assimp/mesh.h>

class AssimpModelLoader
{
public:
    CpuVertex ReadVertex(const aiMesh& mesh, unsigned int vertexIndex);
    CpuMesh ReadMesh(const aiMesh& sourceMesh);

    CpuModel Load(const std::filesystem::path path) ;
    CpuNode ReadNode(const aiNode& sourceNode);
    glm::mat4 ConvertMatrix(const aiMatrix4x4& matrix);
void PrintNode(
    const CpuNode& node,
    const CpuModel& model,
    unsigned int depth = 0);
};