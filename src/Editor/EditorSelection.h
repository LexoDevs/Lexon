#pragma once

#include <cstdint>
#include <optional>

struct EditorSelection
{
    std::optional<uint32_t> selectedObjectId;

    [[nodiscard]]
    bool HasSelection() const
    {
        return selectedObjectId.has_value();
    }

    [[nodiscard]]
    bool IsSelected(uint32_t objectId) const
    {
        return selectedObjectId.has_value() &&
               selectedObjectId.value() == objectId;
    }

    void Select(uint32_t objectId)
    {
        selectedObjectId = objectId;
    }

    void Clear()
    {
        selectedObjectId.reset();
    }
};