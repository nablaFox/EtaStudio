#pragma once

#include "eta_asset.hpp"

struct TextureData;

namespace eta {

class EtaTextureAsset : public EtaAsset {
public:
	using EtaAsset::EtaAsset;
	std::string filePath;
	TextureData* textureData = nullptr; // we can set it manually or load it from the file

	void load() override {}

	AllocatedImage& getImage() { return m_image; }
	VkSampler getSampler() { return m_sampler; }

private:
	AllocatedImage m_image;
	VkSampler m_sampler;
};

}; // namespace eta
