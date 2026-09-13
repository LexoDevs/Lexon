#pragma once

#include <cstddef>

enum class KeyCode
{
    Unknown = 0,

    W,
    A,
    S,
    D,

    Q,
    E,

    H,

    Escape,

    Left,
    Right,
    Up,
    Down,

    Space,

    Count
};

enum class MouseButton
{
    Left = 0,
    Right,
    Middle,

    Count,
    Unknown
};