#pragma once

#include "eta_descriptor.hpp"

struct RenderingConfigs {};

struct PipelineConfigs {
	VkPrimitiveTopology inputTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
	VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	VkFormat colorAttachmentFormat = VK_FORMAT_B8G8R8A8_UNORM;
	VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	VkPipelineRenderingCreateInfo renderInfo = {};
};

namespace eta {

struct GPUDrawPushConstants {
	glm::mat4 modelMatrix;
	VkDeviceAddress vertexBuffer;
};

class EtaPipeline {
public:
	void bindDescriptorSet(VkCommandBuffer cmd, const EtaDescriptorSet& set);
	void bind(VkCommandBuffer cmd);
	virtual void build() = 0;

protected:
	VkPipeline m_pipeline;
	VkPipelineLayout m_pipelineLayout;
};

class EtaGraphicsPipeline : public EtaPipeline {
public:
	void build() override;

	void pushConstants(VkCommandBuffer cmd, GPUDrawPushConstants* constants,
					   VkShaderStageFlags stage = VK_SHADER_STAGE_VERTEX_BIT);

	void setShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
	void setInputTopology(VkPrimitiveTopology topology);
	void setPolygonMode(VkPolygonMode mode);
	void setCullMode(VkCullModeFlags mode, VkFrontFace frontFace);

	void setMultisamplingNone();

	void disableBlending();
	void enableBlendingAdditive();
	void enableBlendingAlphablend();

	void setColorAttachmentFormat(VkFormat format);

	void setDepthFormat(VkFormat format);
	void enableDepthTest(bool enable, VkCompareOp op = VK_COMPARE_OP_LESS);
	void disableDepthTest();

	void setPushConstantRange(VkPushConstantRange& bufferRange, uint32_t count = 1);

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

}; // namespace eta
