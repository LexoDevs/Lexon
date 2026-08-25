
#pragma once
#include <string>
#include <cstdint>


struct WindowProperties {
    std::string title = "Lexon Engine";
    int width = 1280;
    int height = 720;
    bool fullscreen = false;
    bool resizable = true;
    bool vsync = true;
};