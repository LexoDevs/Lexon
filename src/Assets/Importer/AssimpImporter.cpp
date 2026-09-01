#include "AssimpImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stdexcept>
#include <string>

#include <iostream>

CpuVertex AssimpModelLoader::ReadVertex(const aiMesh& mesh, unsigned int vertexIndex)
    {
        CpuVertex vertex{};

        const aiVector3D& position = mesh.mVertices[vertexIndex];

        vertex.position = {
            position.x,
            position.y,
            position.z
        };

        if (mesh.HasNormals())
        {
            const aiVector3D& normal = mesh.mNormals[vertexIndex];

            vertex.normal = {
                normal.x,
                normal.y,
                normal.z
            };
        }

        if (mesh.HasTextureCoords(0))
        {
            const aiVector3D& uv = mesh.mTextureCoords[0][vertexIndex];

            vertex.uv = {
                uv.x,
                uv.y
            };
        }

        return vertex;
    };

CpuMesh AssimpModelLoader::ReadMesh(const aiMesh& sourceMesh)
    {
        CpuMesh result{};

        result.name = sourceMesh.mName.C_Str();
        result.materialIndex = sourceMesh.mMaterialIndex;

        result.vertices.reserve(sourceMesh.mNumVertices);

        for (unsigned int i = 0; i < sourceMesh.mNumVertices; ++i)
        {
            result.vertices.push_back(ReadVertex(sourceMesh, i));
        }

        result.indices.reserve(
            static_cast<std::size_t>(sourceMesh.mNumFaces) * 3
        );

        for (unsigned int faceIndex = 0;
             faceIndex < sourceMesh.mNumFaces;
             ++faceIndex)
        {
            const aiFace& face = sourceMesh.mFaces[faceIndex];

            if (face.mNumIndices != 3)
            {
                throw std::runtime_error(
                    "Assimp devolvio una cara no triangular"
                );
            }

            for (unsigned int i = 0; i < face.mNumIndices; ++i)
            {
                const unsigned int index = face.mIndices[i];

                if (index >= sourceMesh.mNumVertices)
                {
                    throw std::runtime_error(
                        "Assimp devolvio un indice fuera de rango"
                    );
                }

                result.indices.push_back(
                    static_cast<uint32_t>(index)
                );
            }
        }

        return result;
    };



CpuModel AssimpModelLoader::Load(std::filesystem::path path) {

    std::cout << "\033[1;36m[!] Cargando modelo fbx...\033[0m\n";
    std::cout << path.string() << std::endl;

    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error(
            "El modelo no existe: " + path.string()
        );
    }

    if (!std::filesystem::is_regular_file(path))
    {
        throw std::runtime_error("La ruta no corresponde a un archivo: " + path.string());
    }
        
    Assimp::Importer importer;

    constexpr unsigned int flags =
    aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices |
    aiProcess_GenSmoothNormals |
    aiProcess_ImproveCacheLocality |
    aiProcess_ValidateDataStructure |
    aiProcess_FlipUVs;

    const aiScene* scene = importer.ReadFile(path.string(), flags);

    if (scene == nullptr)
    {
        throw std::runtime_error(
            "Assimp no pudo importar '" +
            path.string() +
            "': " +
            importer.GetErrorString()
        );
    }

    if ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
    {
        throw std::runtime_error(
            "Assimp ha devuelto una escena incompleta: " +
            std::string(importer.GetErrorString())
        );
    }

        if (scene->mRootNode == nullptr)
    {
        throw std::runtime_error(
            "El modelo no contiene un nodo raiz"
        );
    }

    CpuModel result{};

    result.sourcePath =
        std::filesystem::weakly_canonical(path.string());

    result.meshes.reserve(scene->mNumMeshes);

    std::cout<<"Numero de meshes detectados: "<<scene->mNumMeshes<<std::endl;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        if (scene->mMeshes[i] == nullptr)
        {
            throw std::runtime_error(
                "La escena contiene un mesh nulo"
            );
        }

        result.meshes.push_back(
            ReadMesh(*scene->mMeshes[i])
        );
    }
        result.materialNames.reserve(scene->mNumMaterials);

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        aiString materialName;

        if (scene->mMaterials[i] != nullptr &&
            scene->mMaterials[i]->Get(
                AI_MATKEY_NAME,
                materialName) == AI_SUCCESS)
        {
            result.materialNames.emplace_back(
                materialName.C_Str()
            );
        }
        else
        {
            result.materialNames.emplace_back(
                "Material_" + std::to_string(i)
            );
        }
    }

    result.rootNode = ReadNode(*scene->mRootNode);

    // A partir de aquí result ya no depende de Assimp.
    // El importer puede destruirse con seguridad.

    return result; 
};

void AssimpModelLoader::PrintNode(
    const CpuNode& node,
    const CpuModel& model,
    unsigned int depth)
{
    const std::string indentation(depth * 2, ' ');

    std::cout
        << indentation
        << "Nodo: "
        << node.name
        << '\n';

    for (uint32_t meshIndex : node.meshIndices)
    {
        if (meshIndex >= model.meshes.size())
        {
            std::cout
                << indentation
                << "  Mesh invalido: "
                << meshIndex
                << '\n';

            continue;
        }

        const CpuMesh& mesh = model.meshes[meshIndex];

        std::cout
            << indentation
            << "  Mesh: "
            << mesh.name
            << " | vertices="
            << mesh.vertices.size()
            << " | indices="
            << mesh.indices.size()
            << " | material="
            << mesh.materialIndex
            << '\n';
    }

    for (const CpuNode& child : node.children)
    {
        PrintNode(child, model, depth + 1);
    }
}


CpuNode AssimpModelLoader::ReadNode(const aiNode& sourceNode)
    {
        CpuNode result{};

        result.name = sourceNode.mName.C_Str();
        result.localTransform =
            ConvertMatrix(sourceNode.mTransformation);

        result.meshIndices.reserve(sourceNode.mNumMeshes);

        for (unsigned int i = 0; i < sourceNode.mNumMeshes; ++i)
        {
            result.meshIndices.push_back(
                static_cast<uint32_t>(sourceNode.mMeshes[i])
            );
        }

        result.children.reserve(sourceNode.mNumChildren);

        for (unsigned int i = 0; i < sourceNode.mNumChildren; ++i)
        {
            if (sourceNode.mChildren[i] == nullptr)
            {
                continue;
            }

            result.children.push_back(
                ReadNode(*sourceNode.mChildren[i])
            );
        }

        return result;
    }


glm::mat4 AssimpModelLoader::ConvertMatrix(const aiMatrix4x4& matrix)
    {
        glm::mat4 result{1.0f};

        // GLM utiliza result[columna][fila].
        result[0][0] = matrix.a1;
        result[1][0] = matrix.a2;
        result[2][0] = matrix.a3;
        result[3][0] = matrix.a4;

        result[0][1] = matrix.b1;
        result[1][1] = matrix.b2;
        result[2][1] = matrix.b3;
        result[3][1] = matrix.b4;

        result[0][2] = matrix.c1;
        result[1][2] = matrix.c2;
        result[2][2] = matrix.c3;
        result[3][2] = matrix.c4;

        result[0][3] = matrix.d1;
        result[1][3] = matrix.d2;
        result[2][3] = matrix.d3;
        result[3][3] = matrix.d4;

        return result;
    }