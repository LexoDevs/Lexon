#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.h>

#include <array>


// Define the number of objects to render




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
        glm::mat4 getModelMatrix() const;
        const std::vector<Vertex>& getVertices() const {return vertices;};
        const std::vector<uint32_t>& getIndices() const {return indices;};
        void setVertices(std::vector<Vertex> offset) { vertices = offset; }
        void setIndices(std::vector<uint32_t> count)   { indices = count; }

        void setupGameObjects();
        void AddObject(LoaderAssets loader, int orden);

    std::vector<VkBuffer>& getuniformBuffersmesh(){ return uniformBuffersmesh; };
    std::vector<VkDeviceMemory>& getuniformBuffersMemorymesh(){ return uniformBuffersMemorymesh; };
    VkDeviceMemory& getuniformBuffersMemoryValue(int i){ return uniformBuffersMemorymesh[i]; };

    std::vector<void*>& getuniformBuffersMappedmesh(){ return uniformBuffersMappedmesh; };
    const std::vector<VkDescriptorSet>&  getdescriptorSetsmesh() const { return descriptorSetsmesh; };
 std::vector<VkDescriptorSet>&  getdescriptorSetsmesh()  { return descriptorSetsmesh; };

    glm::vec3& getposition () { return position;};
    glm::vec3& getrotation () { return rotation;};
    glm::vec3& getscale () { return scale;};
    
    void setposition (glm::vec3 pos) {  position = pos;};
    void setrotation (glm::vec3 rot) {  rotation = rot;};
    void setscale (glm::vec3 scal) {  scale = scal;};
    
    uint32_t getIndexOffset() const { return indexOffset; }
    uint32_t getIndexCount()  const { return indexCount; }
    void setIndexOffset(uint32_t offset) { indexOffset = offset; }
    void setIndexCount(uint32_t count)   { indexCount = count; }

    void PrepareLevelObjects(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);



    private:
    uint32_t indexOffset = 0;  
    uint32_t indexCount  = 0;

    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    std::vector<VkBuffer> uniformBuffersmesh;
    std::vector<VkDeviceMemory> uniformBuffersMemorymesh;
    std::vector<void*> uniformBuffersMappedmesh;

    std::vector<VkDescriptorSet> descriptorSetsmesh;

//Creado ya y usado
    std::vector<Vertex> vertices = {};
    std::vector<uint32_t> indices = {};
};

constexpr int MAX_OBJECTS = 3;

// In the VulkanApplication class:
// Array of game objects to render
static std::array<ObjectInstance, MAX_OBJECTS> LevelObjects;