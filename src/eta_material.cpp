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

void EtaMaterial::setTexture(int binding, str textureName) {
	auto texture = m_manager.getAsset<EtaTextureAsset>(textureName);

	if (texture == nullptr)
		fmt::print("Texture {} not found\n", textureName);

	addTextureBinding(binding, texture);
}

void EtaMaterial::load() {
	if (m_descriptorAllocator == nullptr)
		m_descriptorAllocator = m_device.getGlobalDescriptorAllocator();

	// print texture size
	EtaBindings::init(m_device, *m_descriptorAllocator);
}

void EtaMaterial::destroy() { EtaBindings::destroy(m_device); }
