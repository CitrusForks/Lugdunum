#include <lug/Graphics/Vulkan/Pipeline.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Pipeline::Pipeline(VkPipeline pipeline, const Device* device,
    std::unique_ptr<PipelineLayout> layout, std::unique_ptr<RenderPass> renderPass)
: _pipeline(pipeline), _device(device), _layout(std::move(layout)), _renderPass(std::move(renderPass)) {}

Pipeline::Pipeline(Pipeline&& pipeline) {
    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    _layout = std::move(pipeline._layout);
    _renderPass = std::move(pipeline._renderPass);
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;
    pipeline._layout = nullptr;
    pipeline._renderPass = nullptr;
}

Pipeline& Pipeline::operator=(Pipeline&& pipeline) {
    destroy();

    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    _layout = std::move(pipeline._layout);
    _renderPass = std::move(pipeline._renderPass);
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;
    pipeline._layout = nullptr;
    pipeline._renderPass = nullptr;

    return *this;
}

Pipeline::~Pipeline() {
    destroy();
}

void Pipeline::destroy() {
    if (_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(*_device, _pipeline, nullptr);
        _pipeline = VK_NULL_HANDLE;
    }

    if (_layout != nullptr) {
        _layout->destroy();
    }

    if (_renderPass != nullptr) {
        _renderPass->destroy();
    }
}

RenderPass* Pipeline::getRenderPass() const {
    return _renderPass.get();
}

void Pipeline::bind(const CommandBuffer* commandBuffer) {
    vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
}

std::unique_ptr<Pipeline> Pipeline::createGraphicsPipeline(const Device* device, const Swapchain& swapChain) {
    auto vertexShader = ShaderModule::create("shader.vert.spv", device);
    auto fragmentShader = ShaderModule::create("shader.frag.spv", device);
    if (vertexShader == nullptr || fragmentShader == nullptr) {
        return nullptr;
    }

    // Vertex shader stage
    VkPipelineShaderStageCreateInfo vertexShaderStage{
        vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        vertexShaderStage.pNext = nullptr,
        vertexShaderStage.flags = 0,
        vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT,
        vertexShaderStage.module = *vertexShader,
        vertexShaderStage.pName = "main",
        vertexShaderStage.pSpecializationInfo = nullptr
    };

    // Fragment shader stage
    VkPipelineShaderStageCreateInfo fragmentShaderStage{
        fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        fragmentShaderStage.pNext = nullptr,
        fragmentShaderStage.flags = 0,
        fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        fragmentShaderStage.module = *fragmentShader,
        fragmentShaderStage.pName = "main",
        fragmentShaderStage.pSpecializationInfo = nullptr
    };

    VkPipelineShaderStageCreateInfo shaderStages[]{
        vertexShaderStage,
        fragmentShaderStage
    };


    // Vertex input state
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        vertexInputInfo.pNext = nullptr,
        vertexInputInfo.flags = 0,
        vertexInputInfo.vertexBindingDescriptionCount = 0,
        vertexInputInfo.pVertexBindingDescriptions = 0,
        vertexInputInfo.vertexAttributeDescriptionCount = 0,
        vertexInputInfo.pVertexAttributeDescriptions = 0
   };

    // Input assembly state
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        inputAssemblyInfo.pNext = nullptr,
        inputAssemblyInfo.flags = 0,
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE // because VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };

    auto& extent = swapChain.getExtent();

    VkViewport viewport{
        viewport.x = 0.0f,
        viewport.y = 0.0f,
        viewport.width = static_cast<float>(extent.width),
        viewport.height = static_cast<float>(extent.height),
        viewport.minDepth = 0.0f,
        viewport.maxDepth = 1.0f,
    };

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportState{
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        viewportState.pNext = nullptr,
        viewportState.flags = 0,
        viewportState.viewportCount = 1,
        viewportState.pViewports = &viewport,
        viewportState.scissorCount = 1,
        viewportState.pScissors = &scissor
    };

    VkPipelineRasterizationStateCreateInfo rasterizer{
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        rasterizer.pNext = nullptr,
        rasterizer.flags = 0,
        rasterizer.depthClampEnable = VK_FALSE,
        rasterizer.rasterizerDiscardEnable = VK_FALSE,
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL,
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT,
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE,
        rasterizer.depthBiasEnable = VK_FALSE,
        rasterizer.depthBiasConstantFactor = 0.0f,
        rasterizer.depthBiasClamp = 0.0f,
        rasterizer.depthBiasSlopeFactor = 0.0f,
        rasterizer.lineWidth = 1.0f
    };

    VkPipelineMultisampleStateCreateInfo multisampling{
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        multisampling.pNext = nullptr,
        multisampling.flags = 0,
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        multisampling.sampleShadingEnable = VK_FALSE,
        multisampling.minSampleShading = 0.0f,
        multisampling.pSampleMask = nullptr,
        multisampling.alphaToCoverageEnable = VK_FALSE,
        multisampling.alphaToOneEnable = VK_FALSE
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment{
        colorBlendAttachment.blendEnable = VK_FALSE,
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO ,
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO ,
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD,
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD,
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo colorBlending{
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        colorBlending.pNext = nullptr,
        colorBlending.flags = 0,
        colorBlending.logicOpEnable = VK_FALSE,
        colorBlending.logicOp = VK_LOGIC_OP_COPY,
        colorBlending.attachmentCount = 1,
        colorBlending.pAttachments = &colorBlendAttachment,
        {} // colorBlending.blendConstants
    };

    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    auto pipelineLayout = PipelineLayout::create(device);
    auto renderPass = RenderPass::create(device, swapChain);

    if (!pipelineLayout || !renderPass) {
        return nullptr;
    }

    VkGraphicsPipelineCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.stageCount = 1,
        createInfo.pStages = shaderStages,
        createInfo.pVertexInputState = &vertexInputInfo,
        createInfo.pInputAssemblyState = &inputAssemblyInfo,
        createInfo.pTessellationState = 0,
        createInfo.pViewportState = &viewportState,
        createInfo.pRasterizationState = &rasterizer,
        createInfo.pMultisampleState = &multisampling,
        createInfo.pDepthStencilState = 0,
        createInfo.pColorBlendState = &colorBlending,
        createInfo.pDynamicState = 0,
        createInfo.layout = *pipelineLayout,
        createInfo.renderPass = *renderPass,
        createInfo.subpass = 0,
        createInfo.basePipelineHandle = VK_NULL_HANDLE,
        createInfo.basePipelineIndex = 0
    };

    VkPipeline pipeline = VK_NULL_HANDLE;

    // TODO: create with VkPipelineCache
    // TODO: create multiple pipelines with one call
    VkResult result = vkCreateGraphicsPipelines(*device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create graphics pipeline: {}", result);
        return nullptr;
    }

    return std::unique_ptr<Pipeline>(new Pipeline(pipeline, device, std::move(pipelineLayout), std::move(renderPass)));
}

} // Vulkan
} // Graphics
} // lug
