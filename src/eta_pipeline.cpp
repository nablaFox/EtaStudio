#include "../include/eta_pipeline.hpp"

using namespace eta;

void EtaPipeline::bindDescriptorSet(VkCommandBuffer cmd, VkDescriptorSet set, uint32_t slot) {
	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, slot, 1, &set, 0, nullptr);
}

void EtaPipeline::addSlotLayout(VkDescriptorSetLayout layout) { m_descriptorSetLayouts.push_back(layout); }

void EtaPipeline::bind(VkCommandBuffer cmd) { vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline); }

void EtaPipeline::setPushConstantRange(VkPushConstantRange& bufferRange, uint32_t count) {
	m_pipelineLayoutInfo.pushConstantRangeCount = count;
	m_pipelineLayoutInfo.pPushConstantRanges = &bufferRange;
}
