#pragma once

namespace eta {

class EtaDevice;

struct EtaDescriptorLayout {
	void addBinding(uint32_t binding, VkDescriptorType type, VkShaderStageFlags shaderStage);
	VkResult allocate(EtaDevice& device);
	void destroy(EtaDevice& device);

	VkDescriptorSetLayout& get() { return _descriptorSetLayout; }

	std::vector<VkDescriptorSetLayoutBinding> _bindings;
	VkDescriptorSetLayout _descriptorSetLayout;
};

struct EtaDescriptorSet {
	void writeImage(uint32_t binding,
					VkImageView imageView,
					VkSampler sampler,
					VkImageLayout layout,
					VkDescriptorType type);

	void writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type);

	VkDescriptorSet& get() { return _descriptorSet; }

	void update(EtaDevice device);

	void printWrites() {
		for (auto& write : _writes) {
			if (write.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				fmt::print("Binding: {}, Offset: {}, Size: {}\n", write.dstBinding, write.pBufferInfo->offset,
						   write.pBufferInfo->range);
		}
	}

private:
	std::vector<std::shared_ptr<VkDescriptorBufferInfo>> _bufferInfos;
	std::vector<std::shared_ptr<VkDescriptorImageInfo>> _imageInfos;
	std::vector<VkWriteDescriptorSet> _writes;
	VkDescriptorSet _descriptorSet;
};

struct EtaDescriptorAllocator {
	struct PoolSizeRatio {
		VkDescriptorType type;
		float ratio;
	};

	VkDescriptorPool m_pool;
	VkResult initPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
	VkResult resetDescriptorPool(EtaDevice& device);
	VkResult allocate(EtaDevice& device, const EtaDescriptorLayout& descriptorLayout, EtaDescriptorSet& descriptorSet);
	void destroyPoll(EtaDevice& device);
};

};	// namespace eta
