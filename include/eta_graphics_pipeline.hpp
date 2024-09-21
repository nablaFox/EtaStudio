#pragma once

#include "eta_pipeline.hpp"
#include "eta_descriptor.hpp"

namespace eta {

class EtaGraphicsPipeline : public EtaPipeline {
public:
	EtaGraphicsPipeline();

	VkResult build(VkDevice device);

	void setShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
	void setInputTopology(VkPrimitiveTopology topology);
	void setPolygonMode(VkPolygonMode mode);
	void setCullMode(VkCullModeFlags mode, VkFrontFace frontFace);

	void setColorAttachmentFormat(VkFormat format);
	void setDepthFormat(VkFormat format);

	void setDepthTest(VkCompareOp op = VK_COMPARE_OP_GREATER_OR_EQUAL);
	void disableDepthTest();

	void setMultisampling(VkSampleCountFlagBits samples);
	void disableMultisampling();

	void disableBlending();
	void setBlendingAdditive();
	void setBlendingAlphablend();

private:
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

}  // namespace eta
