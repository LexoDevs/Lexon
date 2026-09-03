#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>

#include <filesystem>
#include <CPUModel.h>

struct MeshesList{
std::string path;
std::string name;

};

static std::vector<std::string> LoadTexturesFolder()
{
    namespace fs = std::filesystem;

    std::vector<std::string> textures;

    for (const auto& entry : fs::directory_iterator("../resources/Textures/spoonza/"))
    {
        if (entry.path().extension() == ".tga")
            textures.push_back(entry.path().string());
    }

    std::sort(textures.begin(), textures.end());

    return textures;
}


static std::vector<MeshesList> LoaderList = {
    {"../resources/models/sponza.obj" , "Cuarto Vikingo"}
     //  ,{"../src/Meshes/cube.obj" , "Cubo"}
} ;


const std::vector<std::string> TEXTURE_PATHS = LoadTexturesFolder();

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

class LoaderAssets {
    public:
        void LoadModel(std::string path, std::vector<Vertex>& vertex , std::vector<uint32_t>& index);

    private:


};


class ObjectInstance {
    public:
        void AddObject(LoaderAssets loader);
        void SetMatrixModel(std::vector<CpuMesh>& ubo, float time);
        static void TranslateModel(glm::vec3 directionMov);
        static void RotateModel(float angulo, char eje);

        std::vector<Vertex>& getVertices() {return vertices;};
        std::vector<uint32_t>& getIndices() {return indices;};

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        static glm::vec3 position;
        static glm::vec3 rotation;

        static float yaw;
        static float pitch;
};

