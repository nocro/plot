#include "VulkanRender.hpp"


VulkanRender::VulkanRender()
{
    try{
        m_window = std::make_unique<Window>(m_context.m_instance) ;
    }catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


void VulkanRender::drawLine(const Line &l)
{
    
    std::vector<Vertex> vertices;
    for( unsigned int i = 0; i< l.m_x.size(); i++ )
    {
        vertices.push_back({{l.m_x[i], l.m_y[i]}, {1.0f,1.0f,1.0f}});
    }

    std::vector<uint16_t> indices;
    for( unsigned int i = 0; i< l.m_x.size()-1; i++ )
    {
        indices.push_back(i);
        indices.push_back(i+1);
    }

    try{
        m_window->createUniformBuffers(sizeof(UniformBufferObject));
        m_window->addData(vertices, indices);
        
        UniformBufferObject ubo = {};
        ubo.model = glm::mat4(1.0f);
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), 800 / (float)600, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        m_window->updateUniformBuffer(ubo);
        m_window->draw(indices.size());
        while (!glfwWindowShouldClose(m_window->m_window)) {
            glfwPollEvents();
            // m_window->draw(indices.size());

            
        }
    }catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
}


VulkanRender::~VulkanRender()
{

}

void VulkanRender::draw()
{

}

void VulkanRender::draw(GlobalSetting s)
{

}


void VulkanRender::drawHist(const Hist &h)
{

}


void VulkanRender::addLine(const Line &l)
{

}

void VulkanRender::addHist(const Hist&h)
{

}

void VulkanRender::clear()
{

}

void VulkanRender::addHistFollowX(const Hist &h)
{

}
void VulkanRender::addHistOther(const Hist &h)
{

}

void VulkanRender::makeGlobalSettings(GlobalSetting s)
{

}