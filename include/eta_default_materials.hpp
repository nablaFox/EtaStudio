#pragma once

#include "eta_material.hpp"

namespace eta {

class EtaMetallicMaterial : public EtaMaterial {
	using EtaMaterial::EtaMaterial;

public:
	void setup() override {
		setFragShader("metallic.frag");
		setVertShader("metallic.vert");

		setProperty("baseColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		setProperty("metallic", 0.0f);
		setProperty("roughness", 0.5f);

		setTexture(1, "default_white");
		setTexture(2, "default_normal");
	}
};

}; // namespace eta
