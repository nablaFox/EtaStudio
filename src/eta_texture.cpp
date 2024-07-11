#include "../include/eta_texture.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaTextureAsset::load() {
	if (textureData) {
		ETA_CHECK_MSG(
			m_device.createFilledImage(m_image, textureData, m_image.imageExtent, m_image.imageFormat, m_imageUsageFlags),
			"Failed to upload texture " + getName());
		return;
	}

	// load texture data from file, then create and fill the image
}
