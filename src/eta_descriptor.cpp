#include "../include/eta_descriptor.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaDescriptorLayout::addBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shaderStage) {
	_bindings.push_back({
		.binding = binding,
		.descriptorType = type,
		.descriptorCount = 1,
		.stageFlags = shaderStage,
	});
}

VkResult EtaDescriptorLayout::allocate(EtaDevice& device) {
	VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.bindingCount = (uint32_t)_bindings.size(),
		.pBindings = _bindings.data(),
	};

	VK_RETURN(vkCreateDescriptorSetLayout(device.getDevice(), &layoutCreateInfo, nullptr, &_descriptorSetLayout));
	return VK_SUCCESS;
}

void EtaDescriptorLayout::destroy(EtaDevice& device) {
	vkDestroyDescriptorSetLayout(device.getDevice(), _descriptorSetLayout, nullptr);
}

void EtaDescriptorSet::writeImage(uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout,
								  VkDescriptorType type) {
	auto info = std::make_shared<VkDescriptorImageInfo>(VkDescriptorImageInfo{
		.sampler = sampler,
		.imageView = imageView,
		.imageLayout = layout,
	});

	_imageInfos.push_back(info);

	_writes.push_back({
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.pNext = nullptr,
		.dstSet = _descriptorSet,
		.dstBinding = binding,
		.descriptorCount = 1,
		.descriptorType = type,
		.pImageInfo = info.get(),
	});
}

void EtaDescriptorSet::writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type) {
	auto info = std::make_shared<VkDescriptorBufferInfo>(VkDescriptorBufferInfo{
		.buffer = buffer,
		.offset = offset,
		.range = size,
	});

	_bufferInfos.push_back(info);

	_writes.push_back({
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = _descriptorSet,
		.dstBinding = binding,
		.descriptorCount = 1,
		.descriptorType = type,
		.pBufferInfo = info.get(),
	});
}

void EtaDescriptorSet::update(EtaDevice device) {
	vkUpdateDescriptorSets(device.getDevice(), (uint32_t)_writes.size(), _writes.data(), 0, nullptr);
}

VkResult EtaDescriptorAllocator::initPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios) {
	std::vector<VkDescriptorPoolSize> poolSizes;

	for (PoolSizeRatio ratio : poolRatios) {
		poolSizes.push_back(VkDescriptorPoolSize{
			.type = ratio.type,
			.descriptorCount = uint32_t(ratio.ratio * maxSets),
		});
	}

	VkDescriptorPoolCreateInfo poolInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.flags = 0,
		.maxSets = maxSets,
		.poolSizeCount = (uint32_t)poolSizes.size(),
		.pPoolSizes = poolSizes.data(),
	};

	return vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_pool);
}

VkResult EtaDescriptorAllocator::resetDescriptorPool(EtaDevice& device) {
	vkResetDescriptorPool(device.getDevice(), m_pool, 0);
	return VK_SUCCESS;
}

VkResult EtaDescriptorAllocator::allocate(EtaDevice& device, const EtaDescriptorLayout& descriptorLayout,
										  EtaDescriptorSet& descriptorSet) {
	VkDescriptorSetAllocateInfo allocInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.pNext = nullptr,
		.descriptorPool = m_pool,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptorLayout._descriptorSetLayout,
	};

	return vkAllocateDescriptorSets(device.getDevice(), &allocInfo, &descriptorSet.get());
}

void EtaDescriptorAllocator::destroyPoll(EtaDevice& device) { vkDestroyDescriptorPool(device.getDevice(), m_pool, nullptr); }
