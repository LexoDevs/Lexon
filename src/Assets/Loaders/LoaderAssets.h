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

struct MeshesList{
std::string path;
std::string name;

};

static std::vector<MeshesList> LoaderList = {
    {"../resources/models/viking_room.obj" , "Cuarto Vikingo"},
    {"../src/Meshes/cube.obj" , "Cubo"}
} ;



const std::string TEXTURE_PATH = "../resources/Textures/viking_room.png";

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
        void SetMatrixModel(UniformBufferObject& ubo, float time, glm::vec3 pos);

        std::vector<Vertex>& getVertices() {return vertices;};
        std::vector<uint32_t>& getIndices() {return indices;};

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
};

