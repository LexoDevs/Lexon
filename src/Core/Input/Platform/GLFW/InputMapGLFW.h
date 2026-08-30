#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <InputCodes.h>

KeyCode GLFWKeyToKeyCode(int key)
{
    switch (key)
    {
        case GLFW_KEY_W:
            return KeyCode::W;

        case GLFW_KEY_A:
            return KeyCode::A;

        case GLFW_KEY_S:
            return KeyCode::S;

        case GLFW_KEY_D:
            return KeyCode::D;

        case GLFW_KEY_Q:
            return KeyCode::Q;

        case GLFW_KEY_E:
            return KeyCode::E;

        case GLFW_KEY_ESCAPE:
            return KeyCode::Escape;

        case GLFW_KEY_LEFT:
            return KeyCode::Left;

        case GLFW_KEY_RIGHT:
            return KeyCode::Right;

        case GLFW_KEY_UP:
            return KeyCode::Up;

        case GLFW_KEY_DOWN:
            return KeyCode::Down;
    }

    return KeyCode::Unknown;
}

