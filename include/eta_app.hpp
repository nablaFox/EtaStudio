#pragma once

#include "eta_system.hpp"
#include "eta_asset_manager.hpp"
#include "eta_engine.hpp"

namespace eta {

class EtaApp : public EtaEngine {
public:
	void init(const GameEngineSettings& settings = EtaEngine::defaultSettings) {
		EtaEngine::init(settings);
		setup();

		for (auto& system : m_systems)
			system->awake();
	}

	void run() { EtaEngine::run(); }

	void destroy() { EtaEngine::destroy(); }

	virtual void setup() = 0;
};
} // namespace eta
