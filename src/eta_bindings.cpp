#include "../include/eta_bindings.hpp"
#include "../include/eta_device.hpp"
#include "../include/eta_buffer_binding.hpp"
#include "../include/eta_texture.hpp"

using namespace eta;

void EtaBindings::addBufferBinding(int binding, EtaBufferBinding& bufferBinding) {
	m_bufferBindings[binding] = bufferBinding;
	m_descriptorSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
									 VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT);
}

void EtaBindings::addTextureBinding(int binding, std::shared_ptr<EtaTextureAsset> texture) {
	m_descriptorSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
	m_textureBindings[binding] = texture;
}

void EtaBindings::init(EtaDevice& device, EtaDescriptorAllocator& descriptorAllocator) {
	m_descriptorSetLayout.allocate(device);

	descriptorAllocator.allocate(device, m_descriptorSetLayout, m_descriptorSet);

	for (auto& [binding, texture] : m_textureBindings) {
		m_descriptorSet.writeImage(binding, texture->getImage().imageView, texture->getSampler(),
								   texture->getImage().currentLayout);
	}

	m_descriptorSet.update(device);
}

void EtaBindings::updateBuffers(EtaDevice& device) {
	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		bufferBinding.updateBuffer(device, m_descriptorSet);
	}
}

size_t EtaBindings::getUniqueIdentifier() {
	std::string bufferLayout;

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		bufferLayout += std::to_string(binding) + ":";
		bufferLayout += std::to_string(bufferBinding.data.floatProperties.size()) + "f,";
		bufferLayout += std::to_string(bufferBinding.data.vec2Properties.size()) + "v2,";
		bufferLayout += std::to_string(bufferBinding.data.vec3Properties.size()) + "v3,";
		bufferLayout += std::to_string(bufferBinding.data.vec4Properties.size()) + "v4,";
		bufferLayout += std::to_string(bufferBinding.data.mat4Properties.size()) + "m4;";
	}

	std::string textureLayout = std::to_string(m_textureBindings.size());

	return std::hash<std::string>{}(bufferLayout + textureLayout);
}
