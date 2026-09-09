#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

using TextureHandle = std::uint32_t;

enum class TextureColorSpace
{
    SRGB,
    Linear
};

enum class TextureState
{
    Unloaded,
    Loading,
    Ready,
    Failed
};

struct TextureImportSettings
{
    TextureColorSpace colorSpace = TextureColorSpace::SRGB;
    bool generateMipmaps = false;
};

struct CpuTextureData
{
    std::vector<std::uint8_t> pixels;

    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t channels = 4;
};

struct TextureAsset
{
    TextureHandle handle = 0;

    std::filesystem::path sourcePath;
    std::string name;

    TextureImportSettings importSettings;
    TextureState state = TextureState::Unloaded;

    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t mipLevels = 1;

    std::string error;
};