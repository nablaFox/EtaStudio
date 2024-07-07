#include "../include/ets_descriptor_allocator.hpp"

using namespace ets;

VkResult VkeDescriptorAllocator::initPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios) {
	std::vector<VkDescriptorPoolSize> poolSizes;

	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = uint32_t(ratio.ratio * maxSets),
		});
	}

	VkDescriptorPoolCreateInfo poolInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		.maxSets = maxSets,
		.poolSizeCount = (uint32_t)poolSizes.size(),
		.pPoolSizes = poolSizes.data(),
	};

	return vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_pool);
}

VkResult VkeDescriptorAllocator::resetDescriptorPool(VkDevice device) { return vkResetDescriptorPool(device, m_pool, 0); }

VkResult VkeDescriptorAllocator::allocate(VkDevice device, VkeDescriptor* descriptorSet) {
	VkDescriptorSetAllocateInfo allocInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.pNext = nullptr,
		.descriptorPool = m_pool,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptorSet->m_descriptorSetLayout,
	};

	VK_RETURN(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet->m_descriptorSet));

	for (VkWriteDescriptorSet& write : descriptorSet->m_writes) {
		write.dstSet = descriptorSet->m_descriptorSet;
	}

	vkUpdateDescriptorSets(device, (uint32_t)descriptorSet->m_writes.size(), descriptorSet->m_writes.data(), 0, nullptr);

	descriptorSet->m_writes.clear();
	descriptorSet->m_imageInfos.clear();
	descriptorSet->m_bufferInfos.clear();

	return VK_SUCCESS;
}

void VkeDescriptorAllocator::destroyPoll(VkDevice device) { vkDestroyDescriptorPool(device, m_pool, nullptr); }
