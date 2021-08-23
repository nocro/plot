#ifndef UNIFORMBUFFER_HPP
#define UNIFORMBUFFER_HPP
#include "Buffer.hpp"

class UniformBuffer
{
public:
    UniformBuffer(VkDeviceSize& bufferSize, Device &device);
    UniformBuffer(); // ne fait rien 
    void createBuffer(VkDeviceSize& size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Device& device);
    void destroy(VkDevice device);
    void create(VkDeviceSize& bufferSize, Device& device);
    //void createBuffer(VkDeviceSize& size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Device& device); 
private:

public:
    VkBuffer m_buffer; 
    VkDeviceMemory m_bufferMemory;


};



#endif