#pragma once

#include "eta_asset.hpp"

namespace eta {

class EtaShader : public EtaAsset {
public:
	using EtaAsset::EtaAsset;
	std::string filePath;

	void load() override {
		// use m_device to load the shader in the GPU
	}
};

} // namespace eta
