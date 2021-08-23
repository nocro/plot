#ifndef DEVICE_HPP
#define DEVICE_HPP
#include <vulkan/vulkan.h>
#include "Config.hpp"
#include "UtilStruct.hpp"
#include <stdexcept>
#include <set>
#include <iostream>



bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface); 
bool checkDeviceExtensionSupport(VkPhysicalDevice device);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);


class Device 
{

public:
    Device(VkInstance instance, VkSurfaceKHR surface);
    Device();// ne fait absolument rien 
    ~Device();
    
private:
    void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
    void createLogicalDevice(VkInstance instance,VkSurfaceKHR surface);
public:

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE; 
    VkDevice m_device; 

    VkQueue m_graphicsQueue; 
    VkQueue m_presentQueue; 

    

};




#endif