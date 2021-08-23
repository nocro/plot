#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <vector>

const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
inline const bool enableValidationLayers = false;
#else
inline const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
}; /// semble utile  pour 1

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
}; /// semble utile 



#endif