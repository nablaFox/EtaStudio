#pragma once

#include "eta_pipeline.hpp"
#include "eta_utils.hpp"

namespace eta {

struct GraphicsPipelineConfigs {
	VkPrimitiveTopology inputTopology;
	VkPolygonMode polygonMode;
	VkCullModeFlags cullMode;
	VkFrontFace frontFace;
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	VkFormat colorAttachmentFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
	VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;
	bool enableDepthTest = true;
};

class EtaGraphicsPipeline : public EtaPipeline {
public:
	EtaGraphicsPipeline();

	VkResult build(VkDevice device, GraphicsPipelineConfigs configs);

	template <typename... Bindings>
	static size_t calculateHash(GraphicsPipelineConfigs& configs, Bindings&&... bindings) {
		size_t hash = 0;

		etautil::hashCombine(hash, configs.inputTopology);
		etautil::hashCombine(hash, configs.polygonMode);
		etautil::hashCombine(hash, configs.cullMode);
		etautil::hashCombine(hash, configs.frontFace);
		etautil::hashCombine(hash, configs.colorAttachmentFormat);
		etautil::hashCombine(hash, configs.depthFormat);
		etautil::hashCombine(hash, configs.vertexShader);
		etautil::hashCombine(hash, configs.fragmentShader);
		etautil::hashCombine(hash, configs.enableDepthTest);

		(etautil::hashCombine(hash, bindings.getHash()), ...);

		return hash;
	}

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
	void enableDepthTest(bool enable, VkCompareOp op = VK_COMPARE_OP_GREATER_OR_EQUAL);
	void disableDepthTest();

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

} // namespace eta
