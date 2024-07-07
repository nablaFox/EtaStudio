#pragma once

#include "ets_pch.hpp"

namespace ets {

class EtaAsset {
	friend class EtaAssetManager;

public:
	virtual void setup() {}

	str getName() const { return name; }

private:
	std::string name;
};

}; // namespace ets
