#include "ScenePipeline.h"
#include <Core/Renderer/RendererContext.h>

namespace South
{

void ScenePipeline::Init()
{
    //    VkDevice Device = RendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();
    //
    //    const char* ShaderName = "GrayMesh";
    //
    //    VkPipelineShaderStageCreateInfo vertShaderStageInfo =
    //        ShadersLibrary::AddShader(Device, ShaderName, VK_SHADER_STAGE_VERTEX_BIT)->GetInfo();
    //    VkPipelineShaderStageCreateInfo fragShaderStageInfo =
    //        ShadersLibrary::AddShader(Device, ShaderName, VK_SHADER_STAGE_FRAGMENT_BIT)->GetInfo();
    //
    //    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};
    //
    //    VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
    //    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    //
    //    auto bindingDescription    = Vertex::GetBindingDescription();
    //    auto attributeDescriptions = Vertex::GetAttributesDescriptions();
    //
    //    vertexInputInfo.vertexBindingDescriptionCount   = 1;
    //    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    //    vertexInputInfo.pVertexBindingDescriptions      = &bindingDescription;
    //    vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions.data();
    //
    //    VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
    //    inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    //    inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    //    inputAssembly.primitiveRestartEnable = VK_FALSE;
    //
    //    VkPipelineViewportStateCreateInfo viewportState {};
    //    viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    //    viewportState.viewportCount = 1;
    //    viewportState.scissorCount  = 1;
    //
    //    VkPipelineRasterizationStateCreateInfo rasterizer {};
    //    rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    //    rasterizer.depthClampEnable        = VK_FALSE;
    //    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    //    rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
    //    rasterizer.lineWidth               = 1.0f;
    //    rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
    //    rasterizer.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    //    rasterizer.depthBiasEnable         = VK_FALSE;
    //
    //    VkPipelineMultisampleStateCreateInfo multisampling {};
    //    multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    //    multisampling.sampleShadingEnable  = VK_FALSE;
    //    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    //
    //    VkPipelineDepthStencilStateCreateInfo depthStencil {};
    //    depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    //    depthStencil.depthTestEnable       = VK_TRUE;
    //    depthStencil.depthWriteEnable      = VK_TRUE;
    //    depthStencil.depthCompareOp        = VK_COMPARE_OP_LESS;
    //    depthStencil.depthBoundsTestEnable = VK_FALSE;
    //    depthStencil.stencilTestEnable     = VK_FALSE;
    //
    //    VkPipelineColorBlendAttachmentState colorBlendAttachment {};
    //    colorBlendAttachment.colorWriteMask =
    //        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    //    colorBlendAttachment.blendEnable = VK_FALSE;
    //
    //    VkPipelineColorBlendStateCreateInfo colorBlending {};
    //    colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    //    colorBlending.logicOpEnable     = VK_FALSE;
    //    colorBlending.logicOp           = VK_LOGIC_OP_COPY;
    //    colorBlending.attachmentCount   = 1;
    //    colorBlending.pAttachments      = &colorBlendAttachment;
    //    colorBlending.blendConstants[0] = 0.0f;
    //    colorBlending.blendConstants[1] = 0.0f;
    //    colorBlending.blendConstants[2] = 0.0f;
    //    colorBlending.blendConstants[3] = 0.0f;
    //
    //    std::vector<VkDynamicState>      dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    //    VkPipelineDynamicStateCreateInfo dynamicState {};
    //    dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    //    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    //    dynamicState.pDynamicStates    = dynamicStates.data();
    //
    //    constexpr VkPushConstantRange PushConstantRange {
    //        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
    //        .offset     = 0,
    //        .size       = sizeof(glm::mat4),
    //    };
    //
    //    const VkPipelineLayoutCreateInfo PipelineLayoutInfo {
    //        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    //        .pNext                  = nullptr,
    //        .setLayoutCount         = 1,
    //        .pSetLayouts            = &m_DescriptorSetLayout,
    //        .pushConstantRangeCount = 1,
    //        .pPushConstantRanges    = &PushConstantRange,
    //    };
    //
    //    vkCreatePipelineLayout(Device, &PipelineLayoutInfo, nullptr, &m_ViewportPipelineLayout);
    //
    //    VkGraphicsPipelineCreateInfo pipelineInfo {};
    //    pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    //    pipelineInfo.pNext               = nullptr;
    //    pipelineInfo.stageCount          = 2;
    //    pipelineInfo.pStages             = shaderStages;
    //    pipelineInfo.pVertexInputState   = &vertexInputInfo;
    //    pipelineInfo.pInputAssemblyState = &inputAssembly;
    //    pipelineInfo.pViewportState      = &viewportState;
    //    pipelineInfo.pRasterizationState = &rasterizer;
    //    pipelineInfo.pMultisampleState   = &multisampling;
    //    pipelineInfo.pDepthStencilState  = &depthStencil;
    //    pipelineInfo.pColorBlendState    = &colorBlending;
    //    pipelineInfo.pDynamicState       = &dynamicState;
    //    pipelineInfo.layout              = m_ViewportPipelineLayout;
    //    pipelineInfo.renderPass          = m_ViewportRenderPass;
    //    pipelineInfo.subpass             = 0;
    //    pipelineInfo.basePipelineHandle  = nullptr;
    //
    //    vkCreateGraphicsPipelines(Device, nullptr, 1, &pipelineInfo, nullptr, &m_ViewportGraphicsPipelineMesh);
}

} // namespace South