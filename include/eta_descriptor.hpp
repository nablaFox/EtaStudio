#pragma once

#include "eta_pch.hpp"

namespace eta {

class EtaDevice;

struct EtaDescriptorLayout {
	void addBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shaderStage);
	void allocate(EtaDevice& device);

	std::vector<VkDescriptorSetLayoutBinding> _bindings;
	VkDescriptorSetLayout _descriptorSetLayout;
};

struct EtaDescriptorSet {
	void writeImage(uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout);
	void writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type);

	void update(EtaDevice device);

private:
	std::vector<VkWriteDescriptorSet> _writes;
	VkDescriptorSet _descriptorSet;
};

struct EtaDescriptorAllocator {
	struct PoolSizeRatio {
		VkDescriptorType type;
		float ratio;
	};

	VkDescriptorPool m_pool;
	VkResult initPool(EtaDevice& device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
	VkResult resetDescriptorPool(EtaDevice& device);
	VkResult allocate(EtaDevice& device, const EtaDescriptorLayout& descriptorLayout, const EtaDescriptorSet& descriptorSet);
	void destroyPoll(EtaDevice& device);
};

}; // namespace eta
