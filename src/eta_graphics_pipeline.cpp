#include "../include/eta_graphics_pipeline.hpp"
#include "../include/eta_vulkan_init.hpp"

using namespace eta;

EtaGraphicsPipeline::EtaGraphicsPipeline() {
	m_colorBlendAttachment = {};
	m_pipelineLayoutInfo = etainit::graphicsPipelineLayoutCreateInfo();

	m_inputAssembly = {.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};

	m_rasterizer = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};

	m_multisampling = {.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};

	m_depthStencil = {.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};

	m_renderInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO};
}

VkResult EtaGraphicsPipeline::build(VkDevice device, GraphicsPipelineConfigs configs) {
	m_pipelineLayoutInfo.setLayoutCount = m_descriptorSetLayouts.size();
	m_pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();

	VK_RETURN(vkCreatePipelineLayout(device, &m_pipelineLayoutInfo, nullptr, &m_pipelineLayout));

	setShaders(configs.vertexShader, configs.fragmentShader);
	setInputTopology(configs.inputTopology);
	setPolygonMode(configs.polygonMode);
	setCullMode(configs.cullMode, configs.frontFace);
	setColorAttachmentFormat(configs.colorAttachmentFormat);
	setDepthFormat(configs.depthFormat);

	// TEMP
	disableBlending();
	disableDepthTest();
	disableMultisampling();

	VkPipelineViewportStateCreateInfo viewportState = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pNext = nullptr,
		.viewportCount = 1,
		.scissorCount = 1,
	};

	VkPipelineColorBlendStateCreateInfo colorBlending = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = nullptr,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &m_colorBlendAttachment,
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

	VkDynamicState state[2] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

	VkPipelineDynamicStateCreateInfo dynamicState = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = 2,
		.pDynamicStates = state,
	};

	VkGraphicsPipelineCreateInfo pipelineInfo = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = &m_renderInfo,
		.stageCount = (uint32_t)m_shaderStages.size(),
		.pStages = m_shaderStages.data(),
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &m_inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &m_rasterizer,
		.pMultisampleState = &m_multisampling,
		.pDepthStencilState = &m_depthStencil,
		.pColorBlendState = &colorBlending,
		.pDynamicState = &dynamicState,
		.layout = m_pipelineLayout,
	};

	VK_RETURN(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline));

	return VK_SUCCESS;
}

void EtaGraphicsPipeline::setShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader) {
	m_shaderStages.push_back(etainit::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertexShader));
	m_shaderStages.push_back(etainit::pipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragmentShader));
}

void EtaGraphicsPipeline::setInputTopology(VkPrimitiveTopology topology) {
	m_inputAssembly.topology = topology;
	m_inputAssembly.primitiveRestartEnable = VK_FALSE;
}

void EtaGraphicsPipeline::setPolygonMode(VkPolygonMode mode) {
	m_rasterizer.polygonMode = mode;
	m_rasterizer.lineWidth = 1.0f;
}

void EtaGraphicsPipeline::setCullMode(VkCullModeFlags mode, VkFrontFace frontFace) {
	m_rasterizer.cullMode = mode;
	m_rasterizer.frontFace = frontFace;
}

void EtaGraphicsPipeline::disableMultisampling() {
	m_multisampling.sampleShadingEnable = VK_FALSE;
	m_multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	m_multisampling.minSampleShading = 1.0f;
	m_multisampling.pSampleMask = nullptr;
	m_multisampling.alphaToCoverageEnable = VK_FALSE;
	m_multisampling.alphaToOneEnable = VK_FALSE;
}

void EtaGraphicsPipeline::disableBlending() {
	m_colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	m_colorBlendAttachment.blendEnable = VK_FALSE;
}

void EtaGraphicsPipeline::enableBlendingAdditive() {}

void EtaGraphicsPipeline::enableBlendingAlphablend() {}

void EtaGraphicsPipeline::setColorAttachmentFormat(VkFormat format) {
	m_colorAttachmentFormat = format;
	m_renderInfo.colorAttachmentCount = 1;
	m_renderInfo.pColorAttachmentFormats = &m_colorAttachmentFormat;
}

void EtaGraphicsPipeline::setDepthFormat(VkFormat format) { m_renderInfo.depthAttachmentFormat = format; }

void EtaGraphicsPipeline::enableDepthTest(bool enable, VkCompareOp op) {}

void EtaGraphicsPipeline::disableDepthTest() {
	m_depthStencil.depthTestEnable = VK_FALSE;
	m_depthStencil.depthWriteEnable = VK_FALSE;
	m_depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
	m_depthStencil.depthBoundsTestEnable = VK_FALSE;
	m_depthStencil.stencilTestEnable = VK_FALSE;
	m_depthStencil.front = {};
	m_depthStencil.back = {};
	m_depthStencil.minDepthBounds = 0.f;
	m_depthStencil.maxDepthBounds = 1.f;
	m_renderInfo.depthAttachmentFormat = VK_FORMAT_UNDEFINED;
}
