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

struct alignas(16) UniformBufferObject
{
    glm::mat4 view{1.0f};
    glm::mat4 proj{1.0f};

    // xyz = dirección de los rayos
    // w   = intensidad
    alignas(16)
    glm::vec4 sunDirectionIntensity{
        0.0f,
        -1.0f,
        1.0f,
        1.0f
    };

    // rgb = color del sol
    // w   = intensidad ambiente
    alignas(16)
    glm::vec4 sunColorAmbient{
        1.0f,
        0.55f,
        0.20f,
        0.15f
    };
    // x = RenderViewMode
    alignas(16)
    glm::uvec4 renderFlags{0u};
};

static_assert(
    sizeof(UniformBufferObject) % 16 == 0,
    "UniformBufferObject debe estar alineado a 16 bytes"
);


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


