#ifndef CONTEXT_hpp
#define CONTEXT_hpp
#include "Window.hpp"
#include <iostream>
#include <string.h>

bool checkValidationLayerSupport();
std::vector<const char*> getRequiredExtensions();
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

class Context
{
public:

    //glfinit ,initVulkan
    Context();// init juste m_instance 
    ~Context();

    Window createWindow();

private:
    void createInstance();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void setupDebugMessenger();
public:

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }
    

};




#endif