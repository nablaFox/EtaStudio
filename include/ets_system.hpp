#pragma once

#include "ets_scene.hpp"

namespace ets {

class EtaEngine;

class EtaSystem {
public:
	EtaSystem(EtaEngine& engine) : m_engine(engine) {}
	virtual ~EtaSystem() = default;

	virtual void update() {}
	virtual void awake() {}

protected:
	EtaEngine& m_engine;
};

}; // namespace ets
