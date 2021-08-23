#include "UniformBuffer.hpp"

UniformBuffer::UniformBuffer(VkDeviceSize& bufferSize, Device& device)
{
    
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device);

}
UniformBuffer::UniformBuffer()
{
    
}

void UniformBuffer::create(VkDeviceSize& bufferSize, Device& device)
{
   
    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device);

}

void UniformBuffer::createBuffer(VkDeviceSize& size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Device& device) 
{
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device.m_device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device.m_device, m_buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, device.m_physicalDevice);

    if (vkAllocateMemory(device.m_device, &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device.m_device, m_buffer, m_bufferMemory, 0);
    
}

void UniformBuffer::destroy(VkDevice device)
{
    vkDestroyBuffer(device, m_buffer, nullptr);
    vkFreeMemory(device, m_bufferMemory, nullptr);
}