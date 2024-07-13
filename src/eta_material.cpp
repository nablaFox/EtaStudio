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

	EtaBindings::setTextureBinding(binding, texture);
}

void EtaMaterial::load() {
	if (m_descriptorAllocator == nullptr)
		m_descriptorAllocator = m_device.getGlobalDescriptorAllocator();

	EtaBindings::init(m_device, *m_descriptorAllocator);

	fmt::println("Info: Loaded material {}", getName());
}

void EtaMaterial::destroy() { EtaBindings::destroy(m_device); }
