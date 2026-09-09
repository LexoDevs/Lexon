#include "TextureManager.h"
/*
TextureHandle TextureManager::LoadTexture(...)
{
    const auto normalizedPath =
        std::filesystem::weakly_canonical(path);

    const std::string key = normalizedPath.generic_string();

    if (auto found = pathToHandle.find(key);
        found != pathToHandle.end())
    {
        return found->second;
    }

    TextureRecord record;
    record.asset.handle = nextHandle++;
    record.asset.sourcePath = normalizedPath;
    record.asset.name = normalizedPath.filename().string();
    record.asset.importSettings = settings;
    record.asset.state = TextureState::Loading;

    try
    {
        CpuTextureData cpu =
            TextureImporter::Load(normalizedPath, settings);

        record.gpuTexture.Create(
            cpu,
            settings,
            device,
            physicalDevice,
            commandPool,
            graphicsQueue
        );

        record.asset.width = cpu.width;
        record.asset.height = cpu.height;
        record.asset.state = TextureState::Ready;

        // cpu sale de ámbito y libera los píxeles.
    }
    catch (const std::exception& exception)
    {
        record.asset.state = TextureState::Failed;
        record.asset.error = exception.what();
    }

    const TextureHandle handle = record.asset.handle;

    textures.emplace(handle, std::move(record));
    pathToHandle.emplace(key, handle);

    return handle;
}*/