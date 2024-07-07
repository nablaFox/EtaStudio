#pragma once

#include "ets_system.hpp"

namespace ets {

class EtaRenderingSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update() override{
		// Render all entities
	};
};

}; // namespace ets
