#pragma once

#include "eta_pipeline.hpp"

namespace eta {

struct GraphicsPipelineConfigs {
	VkPrimitiveTopology inputTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
	VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkFormat colorAttachmentFormat = VK_FORMAT_B8G8R8A8_UNORM;
	VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
};

struct RenderingConfigs {
	GraphicsPipelineConfigs pipelineConfigs;
	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;
};

class EtaGraphicsPipeline : public EtaPipeline {
public:
	EtaGraphicsPipeline();

	VkResult build(VkDevice device, RenderingConfigs& configs);

private:
	void setShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
	void setInputTopology(VkPrimitiveTopology topology);
	void setPolygonMode(VkPolygonMode mode);
	void setCullMode(VkCullModeFlags mode, VkFrontFace frontFace);

	void disableMultisampling();

	void disableBlending();
	void enableBlendingAdditive();
	void enableBlendingAlphablend();

	void setColorAttachmentFormat(VkFormat format);

	void setDepthFormat(VkFormat format);
	void enableDepthTest(bool enable, VkCompareOp op = VK_COMPARE_OP_LESS);
	void disableDepthTest();

	void setDescriptorSetLayout(const EtaDescriptorLayout& layout);

private:
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
	VkPipelineColorBlendAttachmentState m_colorBlendAttachment;
	VkFormat m_colorAttachmentFormat;
	VkPipelineInputAssemblyStateCreateInfo m_inputAssembly;
	VkPipelineRasterizationStateCreateInfo m_rasterizer;
	VkPipelineMultisampleStateCreateInfo m_multisampling;
	VkPipelineDepthStencilStateCreateInfo m_depthStencil;
	VkPipelineRenderingCreateInfo m_renderInfo;
};

} // namespace eta
