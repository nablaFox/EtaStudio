#include "../include/eta_material.hpp"
#include "../include/eta_asset_manager.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaMaterial::setShader(str shaderPath) {
	m_shader = m_manager.getAsset<EtaShader>(shaderPath);
	if (m_shader == nullptr) {
		fmt::print("Shader {} not found\n", shaderPath);
	}
}

void EtaMaterial::setProperty(str name, float value) { EtaBindings::setBufferProperty(0, name, value); }

void EtaMaterial::setProperty(str name, glm::vec4 value) { EtaBindings::setBufferProperty(0, name, value); }

void EtaMaterial::setProperty(str name, glm::vec3 value) { EtaBindings::setBufferProperty(0, name, value); }

void EtaMaterial::setProperty(str name, glm::vec2 value) { EtaBindings::setBufferProperty(0, name, value); }

void EtaMaterial::setProperty(str name, glm::mat4 value) { EtaBindings::setBufferProperty(0, name, value); }

void EtaMaterial::load() {
	if (m_descriptorAllocator == nullptr)
		m_descriptorAllocator = m_device.getGlobalDescriptorAllocator();

	EtaBindings::init(m_device, *m_descriptorAllocator);
}

void EtaMaterial::destroy() { EtaBindings::destroy(m_device); }

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
