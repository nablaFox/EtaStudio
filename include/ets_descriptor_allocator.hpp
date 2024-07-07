#pragma once

#include "ets_pch.hpp"
#include "ets_descriptor.hpp"

namespace ets {

struct VkeDescriptorAllocator {
	struct PoolSizeRatio {
		VkDescriptorType type;
		float ratio;
	};

	VkDescriptorPool m_pool;
	VkResult initPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
	VkResult resetDescriptorPool(VkDevice device);
	VkResult allocate(VkDevice device, VkeDescriptor* descriptorSet);
	void destroyPoll(VkDevice device);
};

}; // namespace ets
