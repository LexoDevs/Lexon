#pragma once

#include "TextureAsset.h"

class TextureImporter
{
public:
    CpuTextureData Load( const std::filesystem::path& path, const TextureImportSettings& settings);
};