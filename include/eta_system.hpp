#pragma once

#include "eta_scene.hpp"
#include "eta_engine.hpp"

namespace eta {

class EtaEngine;

class EtaSystem {
public:
	EtaSystem(EtaEngine& engine) : m_engine(engine) {}
	virtual ~EtaSystem() = default;

	virtual void update() {}
	virtual void awake() {}
	virtual void sleep() {}

	EtaSceneAsset& currentScene();

protected:
	EtaEngine& m_engine;
};

}; // namespace eta
