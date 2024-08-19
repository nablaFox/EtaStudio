#pragma once

#include "eta_system.hpp"

namespace eta {

class EtaInputSystem : public EtaSystem {
public:
	EtaInputSystem(EtaEngine& engine, EtaWindow& window) : EtaSystem(engine), m_window(window) {}

	void update(float _) override;

private:
	EtaWindow& m_window;
};

}  // namespace eta
