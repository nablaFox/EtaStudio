#pragma once

#include "ets_scene.hpp"

namespace ets {

class EtaInitialScene : public EtaSceneAsset {
public:
	void setup() override {
		fmt::println("EtaInitialScene setup");

		addEntity<CameraComponent>();
		addEntity<PointLightComponent>();
	}
};

}; // namespace ets
