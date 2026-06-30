
#pragma once
#include <string>
#include <cstdint>


struct WindowProperties {
    std::string title = "Lexon Engine";
    uint32_t width = 1280;
    uint32_t height = 720;
    bool fullscreen = false;
    bool resizable = true;
    bool vsync = true;
};