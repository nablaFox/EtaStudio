#pragma once

#include "eta_scene.hpp"

namespace eta {

class EtaInitialScene : public EtaSceneAsset {
public:
	using EtaSceneAsset::EtaSceneAsset;

	void setup() override {
		fmt::println("EtaInitialScene setup");

		addEntity<CameraComponent>();
		addEntity<PointLightComponent>();
	}
};

}; // namespace eta
