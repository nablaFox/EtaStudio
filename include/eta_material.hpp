#pragma once

#include "eta_bindings.hpp"
#include "eta_asset.hpp"

namespace eta {
class EtaShader;

class EtaMaterial : public EtaAsset, public EtaBindings {
	friend class EtaRenderingSystem;

public:
	EtaMaterial(EtaDevice& device, EtaAssetManager& assetManager, str name, AllocatedBuffer buffer, size_t offset)
		: EtaAsset(device, assetManager, name) {
		EtaBindings::addBufferBinding(0, buffer, offset);
	}

	EtaMaterial(EtaDevice& device, EtaAssetManager& assetManager, str name) : EtaAsset(device, assetManager, name) {
		EtaBindings::addBufferBinding(0);
	}

	void setShader(str name);

	void setProperty(str name, glm::vec4 value);

	void setProperty(str name, glm::vec3 value);

	void setProperty(str name, glm::vec2 value);

	void setProperty(str name, float value);

	void setProperty(str name, glm::mat4 value);

	void load() override;

	void destroy() override;

	// TEMP: should call this only on setup
	void setTexture(uint32_t binding, str textureName);

private:
	std::shared_ptr<EtaShader> m_shader;

	AllocatedBuffer m_uboBuffer = {};
	size_t m_bufferOffset = 0;

	std::shared_ptr<EtaDescriptorAllocator> m_descriptorAllocator;
};

}; // namespace eta
