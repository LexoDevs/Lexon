#pragma once

#include "../Assets/Textures/TextureImporter.h"
#include <unordered_map>

class TextureManager
{
public:
    TextureHandle LoadTexture(
        const std::filesystem::path& path,
        TextureImportSettings settings = {}
    ){};

    TextureAsset* GetAsset(TextureHandle handle){};
    //VulkanTexture* GetGPUTexture(TextureHandle handle);

    void Unload(TextureHandle handle){};
    void Shutdown(){};

private:
    struct TextureRecord
    {
        TextureAsset asset;
        //VulkanTexture gpuTexture;
    };

    TextureHandle nextHandle = 1;

    std::unordered_map<TextureHandle, TextureRecord> textures;
    std::unordered_map<std::string, TextureHandle> pathToHandle;
};