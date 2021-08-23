#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Device.hpp"
#include <memory>
#include "Semaphores.hpp"
#include "UniformBuffer.hpp"
#include <fstream>
#include <chrono>







VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
std::vector<char> readFile(const std::string& filename);



class Window
{
public:
    Window(VkInstance& instance, int width = 800, int height = 600);

    // demande les vertices suivit des indices (les indices sont sur 16 bits mais faura changer ca)
    void addData(std::vector<Vertex> vertices, std::vector<uint16_t> indices);
    void draw(std::size_t nbPoint);

    // on met la tailles des donnée a evoyer; 
    // dans l'exemple c'est une structure connu a l'avance
    void createUniformBuffers(VkDeviceSize bufferSize);
    ~Window();

    template<typename T>
    void updateUniformBuffer(T data)
    {
        std::cout << "update uniform buffer"<< std::endl;
        if( m_have_uniform == false )
        {
            std::cout << "no uniform buffer find, useless to update " <<  std::endl;
            return;
        }
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(m_device->m_device, m_swapChain, std::numeric_limits<uint64_t>::max(), m_semaphores->imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            // recreateSwapChain(nbPoint);
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        void* newData;
        vkMapMemory(m_device->m_device, m_uniformBuffers[imageIndex].m_bufferMemory, 0, sizeof(data), 0, &newData);
            memcpy(newData, &data, sizeof(data));
        vkUnmapMemory(m_device->m_device, m_uniformBuffers[imageIndex].m_bufferMemory);

    }

public:

    /*
        pour moi 
    */ 
    bool m_have_uniform= false;
    // si on a ajouter des data et donc si on a deja appeler recreateSwapchain
    bool  m_haveData = false;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSurface();
    void createDescriptorSetLayout();
    void createCommandPool();


    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    
    void createDescriptorPool();
    // on a encore besoin de la taille de l'uniforme fuffer 
    void createDescriptorSets(VkDeviceSize bufferSize);
    // nbPoint = nombres d'indice ak indices.size()
    void createCommandBuffers(std::size_t nbPoint);

    
    void cleanupSwapChain();
    void recreateSwapChain(std::size_t nbPoint);
    // void updateUniformBuffer(uint32_t currentImage);
    

    size_t m_currentFrame = 0;
    
    

public:
    int m_width; 
    int m_height;
    bool m_framebufferResized = false;
    VkInstance &r_instance;

    GLFWwindow* m_window;
    VkSurfaceKHR m_surface;

    // c'est un pointeur 
    std::unique_ptr<Device> m_device;

    std::unique_ptr<Semaphores> m_semaphores;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkCommandPool m_commandPool;

    
    // on les prends au moment du draw 
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer; 

    std::vector<UniformBuffer> m_uniformBuffers; 

    // pour afficher 
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;

    // extent ca veux dire étendue 
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkDescriptorPool m_descriptorPool; 
    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<VkCommandBuffer> m_commandBuffers;

    

    

private: 
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_framebufferResized = true;
    }
    
};







#endif