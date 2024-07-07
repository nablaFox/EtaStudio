#pragma once

#include "eta_pch.hpp"

namespace eta {

class EtaEngine;
class EtaAssetManager;
class EtaDevice;

class EtaAsset {
	friend class EtaAssetManager;

public:
	EtaAsset(EtaDevice& device, EtaAssetManager& manager, const std::string& name)
		: name(name), m_device(device), m_manager(manager) {}

	virtual void setup() {}
	virtual void load() {}

	str getName() const { return name; }

private:
	std::string name;

protected:
	EtaDevice& m_device;
	EtaAssetManager& m_manager;
};

}; // namespace eta
