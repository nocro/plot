#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "Vertex.hpp"
#include "Device.hpp"
#include <vector>
#include <iostream>

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

class VertexBuffer 
{
private:

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, Device &device); 
    
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);

    
public:

    void destroy(VkDevice device);

    template<class T>
    VertexBuffer(std::vector<T> donne, Device &device, VkCommandPool commandPool)
    {
        
        VkDeviceSize bufferSize = sizeof(donne[0]) * donne.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device);
        void* data;
        vkMapMemory(device.m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, donne.data(), (size_t) bufferSize);
        vkUnmapMemory(device.m_device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory, device );

        copyBuffer(stagingBuffer, m_buffer, bufferSize, device.m_device , commandPool, device.m_graphicsQueue);

        vkDestroyBuffer(device.m_device, stagingBuffer, nullptr);
        vkFreeMemory(device.m_device, stagingBufferMemory, nullptr);  
        
    }





public:
    VkBuffer m_buffer; 
    VkDeviceMemory m_bufferMemory; 


};





class IndexBuffer 
{
private:

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, Device &device); 
    
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);

    
public:

    void destroy(VkDevice device);

    template<class T>
    IndexBuffer(std::vector<T> donne, Device &device, VkCommandPool commandPool)
    {
        
        VkDeviceSize bufferSize = sizeof(donne[0]) * donne.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device);
        void* data;
        vkMapMemory(device.m_device, stagingBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, donne.data(), (size_t) bufferSize);
        vkUnmapMemory(device.m_device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory, device );

        copyBuffer(stagingBuffer, m_buffer, bufferSize, device.m_device , commandPool, device.m_graphicsQueue);

        vkDestroyBuffer(device.m_device, stagingBuffer, nullptr);
        vkFreeMemory(device.m_device, stagingBufferMemory, nullptr);  
        
    }





public:
    VkBuffer m_buffer; 
    VkDeviceMemory m_bufferMemory; 


};





#endif