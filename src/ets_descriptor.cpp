#include "../include/ets_descriptor.hpp"

using namespace ets;

VkResult VkeDescriptor::initLayout(VkDevice device, VkShaderStageFlags shaderStages) {
	for (VkDescriptorSetLayoutBinding& binding : m_bindings) {
		binding.stageFlags |= shaderStages;
	}

	VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.bindingCount = (uint32_t)m_bindings.size(),
		.pBindings = m_bindings.data(),
	};

	VK_RETURN(vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, &m_descriptorSetLayout));
	return VK_SUCCESS;
}

void VkeDescriptor::addBinding(uint32_t binding, VkDescriptorType type) {
	m_bindings.push_back({.binding = binding, .descriptorType = type, .descriptorCount = 1});
}

void VkeDescriptor::writeImage(uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout) {
	VkDescriptorImageInfo& info = m_imageInfos.emplace_back(VkDescriptorImageInfo{
		.sampler = sampler,
		.imageView = imageView,
		.imageLayout = layout,
	});

	m_writes.push_back({
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.pNext = nullptr,
		.dstSet = VK_NULL_HANDLE,
		.dstBinding = binding,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		.pImageInfo = &info,
	});
}

void VkeDescriptor::writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type) {
	VkDescriptorBufferInfo& info = m_bufferInfos.emplace_back(VkDescriptorBufferInfo{
		.buffer = buffer,
		.offset = offset,
		.range = size,
	});

	VkWriteDescriptorSet write = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = VK_NULL_HANDLE,
		.dstBinding = binding,
		.descriptorCount = 1,
		.descriptorType = type,
		.pBufferInfo = &info,
	};

	m_writes.push_back(write);
}
