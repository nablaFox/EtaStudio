#include "../include/eta_material.hpp"
#include "../include/eta_asset_manager.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaMaterial::setFragShader(str shaderPath) {
	m_fragShader = m_manager.getAsset<EtaShader>(shaderPath);
	if (m_fragShader == nullptr) {
		fmt::print("Shader {} not found\n", shaderPath);
	}
}

void EtaMaterial::setVertShader(str shaderPath) {
	m_vertShader = m_manager.getAsset<EtaShader>(shaderPath);
	if (m_vertShader == nullptr) {
		fmt::print("Shader {} not found\n", shaderPath);
	}
}

void EtaMaterial::setProperty(str name, glm::vec4 value) { m_bufferBinding.setProperty(name, value); }

void EtaMaterial::setProperty(str name, glm::vec3 value) { m_bufferBinding.setProperty(name, value); }

void EtaMaterial::setProperty(str name, glm::vec2 value) { m_bufferBinding.setProperty(name, value); }

void EtaMaterial::setProperty(str name, float value) { m_bufferBinding.setProperty(name, value); }

void EtaMaterial::setProperty(str name, glm::mat4 value) { m_bufferBinding.setProperty(name, value); }

void EtaMaterial::load() {
	if (m_descriptorAllocator == nullptr) {
		m_descriptorAllocator = std::make_shared<EtaDescriptorAllocator>();
	}

	m_bufferBinding.updateBuffer(m_device, m_uboBuffer, m_descriptorSet, m_bufferOffset);
	addBufferBinding(0, m_bufferBinding);

	EtaBindings::init(m_device, *m_descriptorAllocator);
}

// TEMP: should call this only on setup
void EtaMaterial::setTexture(uint32_t binding, str textureName) {
	if (binding == 0)
		return;

	auto texture = m_manager.getAsset<EtaTextureAsset>(textureName);

	if (texture == nullptr) {
		fmt::print("Texture {} not found\n", textureName);
		return;
	}

	EtaBindings::addTextureBinding(binding, texture);
}
