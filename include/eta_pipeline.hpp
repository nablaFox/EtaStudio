#pragma once

#include "eta_descriptor.hpp"
#include "eta_pch.hpp"

namespace eta {

class EtaPipeline {
public:
	template <typename T>
	void pushConstants(VkCommandBuffer cmd, T& data) {
		static_assert(std::is_standard_layout<T>::value, "T must be standard layout");
		vkCmdPushConstants(cmd, m_pipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeof(T), &data);
	}

	void setPushConstantRange(VkPushConstantRange& bufferRange, uint32_t count = 1);
	void addSlotLayout(VkDescriptorSetLayout layout);
	void bindDescriptorSet(VkCommandBuffer cmd, VkDescriptorSet set, uint32_t slot);
	void bind(VkCommandBuffer cmd);

	void destroy(VkDevice device) {
		vkDestroyPipeline(device, m_pipeline, nullptr);
		vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
	}

protected:
	VkPipeline m_pipeline = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;

	VkPipelineLayoutCreateInfo m_pipelineLayoutInfo;
};

}; // namespace eta
