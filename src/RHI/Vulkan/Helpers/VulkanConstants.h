#pragma once

#include <cstdint> //uint32_t
#include <vector>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<char const*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
const uint32_t MAX_SWAPCHAIN_IMAGES = 3;
