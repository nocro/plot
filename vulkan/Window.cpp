#include "Window.hpp"


Window::Window(VkInstance& instance, int width, int height): 
m_width(width), m_height(height), m_framebufferResized(false), r_instance(instance)
{
    // glfwInit();

    // glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_window = glfwCreateWindow(width, height, "Vulkan 2", nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this); 
    glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);


    createSurface();
    m_device = std::make_unique<Device>(instance, m_surface);
    m_semaphores = std::make_unique<Semaphores>(m_device->m_device);

    createDescriptorSetLayout();
    createCommandPool();

    createSwapChain();
    // recreateSwapChain(0, true);

}

Window::~Window()
{
    //si on a rien afficher ca sert a rien de clean up la swampchain vue qu'on ne l'a pas initioalisé 
    if(m_haveData)
    {
        cleanupSwapChain();
    }
        

    
    vkDestroyDescriptorSetLayout(m_device->m_device, m_descriptorSetLayout, nullptr);

    indexBuffer->destroy(m_device->m_device);
    vertexBuffer->destroy(m_device->m_device);

    m_semaphores->destroy(m_device->m_device);

    vkDestroyCommandPool(m_device->m_device, m_commandPool, nullptr);

    vkDestroySurfaceKHR(r_instance, m_surface, nullptr);

    
        
    glfwDestroyWindow(m_window);
       
}


void Window::draw(std::size_t nbPoint)
{
    if(!m_haveData)
    {
        std::cout<< " no data to print" << std::endl;
        // a la long ecrire ca dans la fenetre
        return;
    }
    else 
    {
        std::cout << "state 1 line 66" << std::endl;
        
        vkWaitForFences(m_device->m_device, 1, &m_semaphores->inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        uint32_t imageIndex;

        std::cout << "state 1,5 line 71" << std::endl;

        VkResult result = vkAcquireNextImageKHR(m_device->m_device, m_swapChain, std::numeric_limits<uint64_t>::max(), m_semaphores->imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
        
        std::cout << "state 1.75 line 75" << std::endl;

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain(nbPoint);
            std::cout<< "recreation de la sap chain" << std::endl ;
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
        
        std::cout << "state 2 line 80" << std::endl;

        //updateUniformBuffer(imageIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {m_semaphores->imageAvailableSemaphores[m_currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {m_semaphores->renderFinishedSemaphores[m_currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(m_device->m_device, 1, &m_semaphores->inFlightFences[m_currentFrame]);

        if (vkQueueSubmit(m_device->m_graphicsQueue, 1, &submitInfo, m_semaphores->inFlightFences[m_currentFrame]) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        std::cout << "state 3 line 107" << std::endl;


        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {m_swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(m_device->m_presentQueue, &presentInfo);
        

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized) {
            m_framebufferResized = false;
            recreateSwapChain(nbPoint); 
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        std::cout << "state 4 line 132" << std::endl;
        m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        std::cout << MAX_FRAMES_IN_FLIGHT << "  " << m_currentFrame <<  std::endl;


        vkDeviceWaitIdle(m_device->m_device);

        std::cout << "end " << std::endl;


    }

}

void Window::recreateSwapChain(std::size_t nbPoint) {
    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(m_device->m_device);


    if(m_haveData)
    {
        cleanupSwapChain();
    }
    createSwapChain();

    createImageViews();
    createRenderPass();
    createGraphicsPipeline();

    createFramebuffers();

    // createUniformBuffers(sizeof(UniformBufferObject));
    

    
    if(m_have_uniform)
    {
        createDescriptorPool();
        createDescriptorSets(sizeof(UniformBufferObject));
    }

    createCommandBuffers(nbPoint);

    
}




void Window::createSurface() {
    if ( glfwCreateWindowSurface(r_instance, m_window, nullptr, &m_surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}



void Window::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(m_device->m_device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}


void Window::createCommandPool() 
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_device->m_physicalDevice, m_surface);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(m_device->m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void Window::addData(std::vector<Vertex> datas, std::vector<uint16_t> indices)
{
    vertexBuffer = std::make_unique<VertexBuffer>(datas, *m_device, m_commandPool );
    indexBuffer = std::make_unique<IndexBuffer>(indices, *m_device, m_commandPool);
    recreateSwapChain(indices.size());
    m_haveData = true;
}



void Window::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_device->m_physicalDevice, m_surface);//use surface too 

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);//use window too 

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(m_device->m_physicalDevice, m_surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(m_device->m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(m_device->m_device, m_swapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_device->m_device, m_swapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;
}



VkExtent2D Window::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}


void Window::createImageViews() {
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = m_swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = m_swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(m_device->m_device, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

void Window::createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(m_device->m_device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}


void Window::createGraphicsPipeline() {
    auto vertShaderCode = readFile("vulkan/shaders/vert.spv");
    auto fragShaderCode = readFile("vulkan/shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);// use device  
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) m_swapChainExtent.width;
    viewport.height = (float) m_swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = m_swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;

    if (vkCreatePipelineLayout(m_device->m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(m_device->m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(m_device->m_device, fragShaderModule, nullptr);
    vkDestroyShaderModule(m_device->m_device, vertShaderModule, nullptr);
}


VkShaderModule Window::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(m_device->m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void Window::createFramebuffers() {
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            m_swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = m_swapChainExtent.width;
        framebufferInfo.height = m_swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(m_device->m_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}


void Window::createUniformBuffers(VkDeviceSize bufferSize) {
    // std::cout << m_swapChainImages.size() << std::endl;
    m_uniformBuffers.resize(m_swapChainImages.size());
    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        //UniformBuffer buf(bufferSize, *m_device);
        m_uniformBuffers[i].create(bufferSize, *m_device);
    }

    m_have_uniform = true;
}

void Window::createDescriptorPool() {
    VkDescriptorPoolSize poolSize = {};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(m_swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(m_swapChainImages.size());

    if (vkCreateDescriptorPool(m_device->m_device, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Window::createDescriptorSets(VkDeviceSize bufferSize) {
    // std::cout << "je rentre dans la creation du descriptor set" << std::endl;
    std::vector<VkDescriptorSetLayout> layouts(m_swapChainImages.size(), m_descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();


    m_descriptorSets.resize(m_swapChainImages.size());
    if (vkAllocateDescriptorSets(m_device->m_device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }
    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        VkDescriptorBufferInfo bufferInfo = {};
        bufferInfo.buffer = m_uniformBuffers[i].m_buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = bufferSize;


        

        VkWriteDescriptorSet descriptorWrite = {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        vkUpdateDescriptorSets(m_device->m_device, 1, &descriptorWrite, 0, nullptr);
    }
    // std::cout << "fin du descriptor set" << std::endl;
}

void Window::createCommandBuffers(std::size_t nbPoint) {
    // std::cout << "je rentre dans la creation du commande buffer " << std::endl ;
    
    m_commandBuffers.resize(m_swapChainFramebuffers.size());
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

    if (vkAllocateCommandBuffers(m_device->m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < m_commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_renderPass;
        renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_swapChainExtent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;
        vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

           
            if(m_haveData)
            {
                VkBuffer vertexBuffers[] = {vertexBuffer->m_buffer};
                VkDeviceSize offsets[] = {0};
                vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

                vkCmdBindIndexBuffer(m_commandBuffers[i], indexBuffer->m_buffer, 0, VK_INDEX_TYPE_UINT16);
                if(m_have_uniform)
                    vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSets[i], 0, nullptr);

                vkCmdDrawIndexed(m_commandBuffers[i], static_cast<uint32_t>(nbPoint), 1, 0, 0, 0);
            }
        vkCmdEndRenderPass(m_commandBuffers[i]);
        if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}


void Window::cleanupSwapChain() 
{

    for (auto framebuffer : m_swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device->m_device, framebuffer, nullptr);
    }
    vkFreeCommandBuffers(m_device->m_device, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

    
    vkDestroyPipeline(m_device->m_device, m_graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(m_device->m_device, m_pipelineLayout, nullptr);
    vkDestroyRenderPass(m_device->m_device, m_renderPass, nullptr);
    
    
    for (auto imageView : m_swapChainImageViews) {
        vkDestroyImageView(m_device->m_device, imageView, nullptr);
    }
    
    
    vkDestroySwapchainKHR(m_device->m_device, m_swapChain, nullptr);

    if(m_have_uniform)
    {
        for (size_t i = 0; i < m_swapChainImages.size(); i++) {
            m_uniformBuffers[i].destroy(m_device->m_device);
        }
    }
    
    
    vkDestroyDescriptorPool(m_device->m_device, m_descriptorPool, nullptr);
}





std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}


VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}