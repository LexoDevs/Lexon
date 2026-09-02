#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

//Vertice unitario cargado en memoria
struct CpuVertex
{
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f};
    glm::vec2 uv{0.0f};
};

//Conjunto de vertices que forman un mesh
struct CpuMesh
{
    std::string name;

    std::vector<CpuVertex> vertices;
    std::vector<uint32_t> indices;

    uint32_t materialIndex = 0;
};


struct CpuNode
{
    std::string name;

    glm::mat4 localTransform{1.0f};

    // Índices dentro de CpuModel::meshes
    std::vector<uint32_t> meshIndices;

    std::vector<CpuNode> children;
};

struct CpuModel
{
    std::filesystem::path sourcePath;

    std::vector<CpuMesh> meshes;
    std::vector<std::string> materialNames;

    CpuNode rootNode;
};

struct GPUMeshRange
{
    uint32_t firstVertex = 0;
    uint32_t vertexCount = 0;

    uint32_t firstIndex = 0;
    uint32_t indexCount = 0;

    uint32_t materialIndex = 0;
};