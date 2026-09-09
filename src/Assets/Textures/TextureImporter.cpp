#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TextureImporter.h"

#include <stdexcept>

CpuTextureData TextureImporter::Load( const std::filesystem::path& path, const TextureImportSettings&)
{
    int width = 0;
    int height = 0;
    int originalChannels = 0;

    stbi_uc* sourcePixels = stbi_load( path.string().c_str(), &width, &height, &originalChannels, STBI_rgb_alpha);

    if (sourcePixels == nullptr)
    {
        throw std::runtime_error( "No se pudo cargar la textura: " + path.string() + " | stb_image: " + stbi_failure_reason());
    }

    const std::size_t imageSize = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;

    CpuTextureData result;
    result.width = static_cast<std::uint32_t>(width);
    result.height = static_cast<std::uint32_t>(height);
    result.channels = 4;

    result.pixels.assign(
        sourcePixels,
        sourcePixels + imageSize
    );

    stbi_image_free(sourcePixels);

    return result;
}