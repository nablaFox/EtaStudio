#pragma once

#include "eta_material.hpp"
#include "eta_asset_manager.hpp"
#include "eta_shader.hpp"

namespace eta {

class EtaMetallicMaterial : public EtaMaterial {
	using EtaMaterial::EtaMaterial;

public:
	void setup() override {
		auto shader = m_manager.addAsset<EtaShader>("default_metallic");

		shader->setVertShader("assets/shaders/default.vert.spv");
		shader->setFragShader("assets/shaders/basic.frag.spv");

		shader->load();

		setShader("default_metallic");

		setProperty("colorFactors", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		setProperty("metalRoughFactors", 0.0f);

		// setTexture(1, "default_white");
		// setTexture(2, "default_normal");

		fmt::println("Loaded default_metallic material");
	}
};

}; // namespace eta
