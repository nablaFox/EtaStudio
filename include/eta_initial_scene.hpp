#pragma once

#include "eta_scene.hpp"

namespace eta {

class EtaInitialScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		fmt::println("EtaInitialScene setup");

		addDefaultCamera();
	}
};

}; // namespace eta
