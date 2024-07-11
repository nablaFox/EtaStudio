#pragma once

#include "eta_scene.hpp"

namespace eta {

class EtaInitialScene : public EtaSceneAsset {
public:
	using EtaSceneAsset::EtaSceneAsset;

	void setup() override {
		fmt::println("EtaInitialScene setup");

		addEntity<CameraComponent>(45.0f,  // fov
								   1.77f,  // aspect
								   0.1f,   // near
								   100.0f, // far
								   true,   // enabled
								   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		addEntity<PointLightComponent>();
	}
};

}; // namespace eta
