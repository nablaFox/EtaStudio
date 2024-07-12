#include "../include/eta_texture.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaTextureAsset::load() {
	ETA_CHECK_MSG(
		m_device.createFilledImage(m_image, textureData, m_image.imageExtent, m_image.imageFormat, VK_IMAGE_USAGE_SAMPLED_BIT),
		"Failed to upload texture " + getName());

	ETA_CHECK_MSG(m_device.createSampler(&m_sampler, &m_samplerCreateInfo), "Failed to create sampler for texture " + getName());

	return;
}
