#pragma once

#include <cstdint>

enum class RenderViewMode : uint32_t
{
    Textured = 0,
    White = 1,

    Count
};

struct RenderSettings
{
    RenderViewMode viewMode =
        RenderViewMode::Textured;

    void CycleViewMode()
    {
        const uint32_t current =
            static_cast<uint32_t>(viewMode);

        const uint32_t count =
            static_cast<uint32_t>(
                RenderViewMode::Count
            );

        viewMode = static_cast<RenderViewMode>(
            (current + 1) % count
        );
    }

    [[nodiscard]]
    uint32_t GetShaderValue() const
    {
        return static_cast<uint32_t>(viewMode);
    }
};