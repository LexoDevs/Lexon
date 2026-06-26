
#include "LoaderAssets.h"



#include <iostream>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

//stbi_uc* pixels = stbi_load(TEXTURE_PATH.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

VkVertexInputBindingDescription Vertex::getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
}


void LoaderAssets::LoadModel(std::string path, std::vector<Vertex>& vertices , std::vector<uint32_t>& indices, int orden){
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err, warn;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw std::runtime_error(err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};


    for (const auto& shape : shapes) {

        for (const auto& index : shape.mesh.indices) {
            
            
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};



            if (uniqueVertices.count(vertex) == 0) {
            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
            vertices.push_back(vertex);

        }

        indices.push_back(uniqueVertices[vertex]);
        }

    }
        std::cout << "Cargado: " << path <<std::endl;
        std::cout <<"Vertices: " << vertices.size() <<std::endl;
        std::cout <<"Indices: " << indices.size() << std::endl;

        LevelObjects[orden].setVertices(vertices);
        LevelObjects[orden].setIndices(indices);



};

void ObjectInstance::setupGameObjects(){
    // Object 1 - Center
    std::cout<<"Primer objeto inicializado"<<std::endl;
    LevelObjects[0].setposition({0.0f, 1.0f, 0.0f});
    LevelObjects[0].getrotation() = {0.0f, 0.0f, 0.0f};
    LevelObjects[0].getscale() = {1.0f, 1.0f, 1.0f};

    // Object 2 - Left
    std::cout<<"Segundo objeto inicializado"<<std::endl;
    LevelObjects[1].getposition() = {-2.0f, 0.0f, -1.0f};
    LevelObjects[1].rotation ={0.0f, 0.0f, 0.0f};
    LevelObjects[1].scale = {0.75f, 0.75f, 0.75f};

    // Object 3 - Right
    std::cout<<"Tercer objeto inicializado"<<std::endl;
    LevelObjects[2].position = {2.0f, 0.0f, -1.0f};
    LevelObjects[2].rotation = {0.0f, 0.0f, 0.0f};
    LevelObjects[2].scale = {0.75f, 0.75f, 0.75f};
};

void ObjectInstance::AddObject(LoaderAssets loader, int orden){

    vertices = getVertices();
    indices = getIndices();
    loader.LoadModel(MODEL_PATH, vertices, indices, orden);


            std::cout<<"Vertices buffers: " <<LevelObjects[orden].getVertices().size()<<std::endl;
        std::cout<<"Indices buffers: " <<LevelObjects[orden].getIndices().size()<<std::endl;


};



glm::mat4 ObjectInstance::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
};



void ObjectInstance::PrepareLevelObjects(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    LoaderAssets loader;

    // === Construir buffers globales + calcular offsets ===


    uint32_t currentIndexOffset = 0;

    for (auto& obj : LevelObjects)
    {
        if (obj.getIndices().empty()) continue;

        uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());

        // Guardar offsets
        obj.setIndexOffset(currentIndexOffset);
        obj.setIndexCount(static_cast<uint32_t>(obj.indices.size()));

        vertices = obj.getVertices();
        indices = obj.getIndices();
        // Añadir vértices


        currentIndexOffset += obj.getIndexCount();



    }
    //setupGameObjects();


    // Subir al GPU (un solo buffer grande)
    //vertexbuffer.uploadVertices(globalVertices);
    //vertexbuffer.uploadIndices(globalIndices);

    std::cout << "=== Escena cargada correctamente ===\n";
    std::cout << "Total Vértices globales: " << vertices.size() << "\n";
    std::cout << "Total Índices globales:  " << indices.size() << std::endl;
}