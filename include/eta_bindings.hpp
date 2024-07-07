#pragma once

#include "eta_pch.hpp"
#include "eta_descriptor.hpp"

namespace eta {

class EtaBufferBinding;
class EtaTextureAsset;
class EtaDevice;

class EtaBindings {

public:
	void addBufferBinding(int binding, EtaBufferBinding& bufferBinding);

	void addTextureBinding(int binding, std::shared_ptr<EtaTextureAsset> texture);

	void init(EtaDevice& device, EtaDescriptorAllocator& descriptorAllocator);

	void updateBuffers(EtaDevice& device);

	size_t getUniqueIdentifier();

protected:
	std::unordered_map<int, EtaBufferBinding> m_bufferBindings;
	std::unordered_map<int, std::shared_ptr<EtaTextureAsset>> m_textureBindings;
	EtaDescriptorSet m_descriptorSet;
	EtaDescriptorLayout m_descriptorSetLayout;
};

} // namespace eta
