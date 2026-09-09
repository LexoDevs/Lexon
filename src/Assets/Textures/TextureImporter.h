#pragma once

#include "TextureAsset.h"

class TextureImporter
{
public:
    static CpuTextureData Load( const std::filesystem::path& path, const TextureImportSettings& settings);
};