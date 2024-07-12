#pragma once

#include "eta_asset.hpp"

namespace eta {

class EtaTextureAsset : public EtaAsset {
public:
	using EtaAsset::EtaAsset;
	std::string filePath;
	void* textureData = nullptr;

	void load() override;

	void setMagFilter(VkFilter filter) { m_samplerCreateInfo.magFilter = filter; }
	void setMinFilter(VkFilter filter) { m_samplerCreateInfo.minFilter = filter; }
	void setAddressModeU(VkSamplerAddressMode mode) { m_samplerCreateInfo.addressModeU = mode; }
	void setAddressModeV(VkSamplerAddressMode mode) { m_samplerCreateInfo.addressModeV = mode; }
	void setAddressModeW(VkSamplerAddressMode mode) { m_samplerCreateInfo.addressModeW = mode; }
	void setMipLodBias(float bias) { m_samplerCreateInfo.mipLodBias = bias; }
	void setAnisotropyEnable(VkBool32 enable) { m_samplerCreateInfo.anisotropyEnable = enable; }
	void setMaxAnisotropy(float maxAnisotropy) { m_samplerCreateInfo.maxAnisotropy = maxAnisotropy; }
	void setCompareEnable(VkBool32 enable) { m_samplerCreateInfo.compareEnable = enable; }
	void setCompareOp(VkCompareOp op) { m_samplerCreateInfo.compareOp = op; }
	void setMinLod(float minLod) { m_samplerCreateInfo.minLod = minLod; }
	void setMaxLod(float maxLod) { m_samplerCreateInfo.maxLod = maxLod; }
	void setBorderColor(VkBorderColor color) { m_samplerCreateInfo.borderColor = color; }
	void setUnnormalizedCoordinates(VkBool32 enable) { m_samplerCreateInfo.unnormalizedCoordinates = enable; }

	void setFormat(VkFormat format) { m_image.imageFormat = format; }
	void setExtent(VkExtent3D extent) { m_image.imageExtent = extent; }

	AllocatedImage& getImage() { return m_image; }
	VkSampler getSampler() { return m_sampler; }

private:
	AllocatedImage m_image;
	VkSampler m_sampler;

	VkSamplerCreateInfo m_samplerCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.mipLodBias = 0.0f,
		// .anisotropyEnable = VK_TRUE,
		.maxAnisotropy = 16,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,
		.minLod = 0.0f,
		.maxLod = 0.0f,
		.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
		.unnormalizedCoordinates = VK_FALSE,
	};
};

}; // namespace eta
