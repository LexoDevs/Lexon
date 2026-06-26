#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.h>

const std::string MODEL_PATH = "../resources/models/viking_room.obj";
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

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}


class LoaderAssets {
    public:
        void LoadModel(std::string path, std::vector<Vertex>& vertex , std::vector<uint32_t>& index, int orden);

    private:


};


class ObjectInstance {
    public:
<<<<<<< HEAD
        glm::mat4 getModelMatrix() const;
        const std::vector<Vertex>& getVertices() const {return vertices;};
        const std::vector<uint32_t>& getIndices() const {return indices;};
        void setVertices(std::vector<Vertex> offset) { vertices = offset; }
        void setIndices(std::vector<uint32_t> count)   { indices = count; }

        void setupGameObjects();
        void AddObject(LoaderAssets loader, int orden);
=======
        void AddObject(LoaderAssets loader);
>>>>>>> parent of 64c4e27 (Primer Game Object (Corregir))

        std::vector<Vertex>& getVertices() {return vertices;};
        std::vector<uint32_t>& getIndices() {return indices;};

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
};