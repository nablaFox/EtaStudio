#pragma once

#include "eta_material.hpp"
#include "eta_asset_manager.hpp"
#include "eta_shader.hpp"

namespace eta {

class EtaBaseTextureColor : public EtaTextureAsset {
	using EtaTextureAsset::EtaTextureAsset;

public:
	void setup() override {
		textureData = reinterpret_cast<uint8_t*>(&m_white);

		setMagFilter(VK_FILTER_NEAREST);
		setMinFilter(VK_FILTER_NEAREST);
		setFormat(VK_FORMAT_R8G8B8A8_UNORM);
		setExtent(1, 1, 1);
	}

private:
	uint32_t m_white = 0xFFFFFFFF;
};

class EtaMetallicRoughnessMaterial : public EtaMaterial {
	using EtaMaterial::EtaMaterial;

public:
	void setup() override {
		if (m_sharedMaterialBuffer.buffer == VK_NULL_HANDLE)
			addBufferBinding(0);
		else
			addBufferBinding(0, m_sharedMaterialBuffer, m_bufferOffset);

		auto shader = m_manager.getAsset<EtaShader>("metallic_roughness");

		if (shader == nullptr) {
			shader = m_manager.addAsset<EtaShader>("metallic_roughness");
			shader->setVertShader("assets/shaders/default.vert.spv");
			shader->setFragShader("assets/shaders/metallic_roughness.frag.spv");
			shader->load();
		}

		setShader(shader);

		addVec4(0, "baseColorFactor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		addFloat(0, "metallicFactor", 0.0f);
		addFloat(0, "roughnessFactor", 1.0f);
		addVec3(0, "emissiveFactor", glm::vec3(0.0f));
		addInt(0, "alphaMode", 0);
		addBool(0, "doubleSided", false);

		setTexture(1, "default_base_color");

		// metallic roughness texture

		// normal texture

		// occlusion texture

		// emissive texture
	}

	void setBaseColorFactor(float a, float b, float c, float d) { setVec4(0, "baseColorFactor", glm::vec4(a, b, c, d)); }

	void setMetallicFactor(float value) { setFloat(0, "metallicFactor", value); }

	void setRoughnessFactor(float value) { setFloat(0, "roughnessFactor", value); }

	void setEmissiveFactor(const glm::vec3& value) { setVec3(0, "emissiveFactor", value); }

	void setMetallicRoughnessTexture(std::shared_ptr<EtaTextureAsset> texture) { setTexture(1, texture->getName()); }

	void setBaseColorTexture(std::shared_ptr<EtaTextureAsset> texture) { setTexture(2, texture->getName()); }

	void printProperties() {
		fmt::print("baseColorFactor: {}\n", m_bufferBindings[0].data.vec4Properties["baseColorFactor"].x,
				   m_bufferBindings[0].data.vec4Properties["baseColorFactor"].y,
				   m_bufferBindings[0].data.vec4Properties["baseColorFactor"].z,
				   m_bufferBindings[0].data.vec4Properties["baseColorFactor"].w);

		fmt::print("metallicFactor: {}\n", m_bufferBindings[0].data.floatProperties["metallicFactor"]);
		fmt::print("roughnessFactor: {}\n", m_bufferBindings[0].data.floatProperties["roughnessFactor"]);
	}

	static inline size_t size =
		sizeof(glm::vec4) + sizeof(float) + sizeof(float) + sizeof(glm::vec3) + sizeof(int) + sizeof(bool);
};

}; // namespace eta
