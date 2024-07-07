#include "../include/eta_descriptor.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

// EtaDescriptorLayout implementation

void EtaDescriptorLayout::addBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shaderStage) {
	// Implementation for adding a descriptor set layout binding
}

void EtaDescriptorLayout::allocate(EtaDevice& device) {
	// Implementation for allocating descriptor set layout
}

// EtaDescriptorSet implementation

void EtaDescriptorSet::writeImage(uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout) {
	// Implementation for writing an image to the descriptor set
}

void EtaDescriptorSet::writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type) {
	// Implementation for writing a buffer to the descriptor set
}

void EtaDescriptorSet::update(EtaDevice device) {
	// Implementation for updating the descriptor set
}

// EtaDescriptorAllocator implementation

VkResult EtaDescriptorAllocator::initPool(EtaDevice& device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios) {
	// Implementation for initializing a descriptor pool
	return VK_SUCCESS;
}

VkResult EtaDescriptorAllocator::resetDescriptorPool(EtaDevice& device) {
	// Implementation for resetting the descriptor pool
	return VK_SUCCESS;
}

VkResult EtaDescriptorAllocator::allocate(EtaDevice& device, const EtaDescriptorLayout& descriptorLayout,
										  const EtaDescriptorSet& descriptorSet) {
	// Implementation for allocating a descriptor set
	return VK_SUCCESS;
}

void EtaDescriptorAllocator::destroyPoll(EtaDevice& device) {
	// Implementation for destroying the descriptor pool
}
