#pragma once

#include "eta_material.hpp"
#include "eta_asset_manager.hpp"
#include "eta_shader.hpp"

namespace eta {

class EtaMetallicMaterial : public EtaMaterial {
	using EtaMaterial::EtaMaterial;

public:
	void setup() override {
		addBufferBinding(0);

		auto shader = m_manager.addAsset<EtaShader>("default_metallic");

		shader->setVertShader("assets/shaders/default.vert.spv");
		shader->setFragShader("assets/shaders/default_metallic.frag.spv");

		shader->load();

		setShader("default_metallic");

		addVec4(0, "colorFactors", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		addFloat(0, "metalRoughFactors", 0.0f);

		// create default white texture
		auto whiteTexture = m_manager.addAsset<EtaTextureAsset>("default_white");
		uint32_t white = 0xFFFFFFFF;
		whiteTexture->textureData = &white;

		whiteTexture->setMagFilter(VK_FILTER_NEAREST);
		whiteTexture->setMinFilter(VK_FILTER_NEAREST);
		whiteTexture->setFormat(VK_FORMAT_R8G8B8A8_UNORM);
		whiteTexture->setExtent({1, 1, 1});

		whiteTexture->load();

		addTextureBinding(1, whiteTexture);

		fmt::println("Loaded default_metallic material");
	}
};

}; // namespace eta
