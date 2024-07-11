#pragma once

#include "eta_scene.hpp"
#include "eta_engine.hpp"

namespace eta {

class EtaEngine;

class EtaSystem {
public:
	EtaSystem(EtaEngine& engine) : m_engine(engine) {}
	virtual ~EtaSystem() = default;

	virtual void update(float delta) {}
	virtual void awake() {}
	virtual void sleep() {}

	EtaScene& currentScene() { return m_engine.currentScene(); }

protected:
	EtaEngine& m_engine;
};

}; // namespace eta
