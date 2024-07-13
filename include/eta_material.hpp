#pragma once

#include "eta_bindings.hpp"
#include "eta_asset.hpp"

namespace eta {
class EtaShader;

class EtaMaterial : public EtaAsset, public EtaBindings {
	friend class EtaRenderingSystem;
	using EtaAsset::EtaAsset;

public:
	void setShader(str name);

	void setShader(std::shared_ptr<EtaShader> shader) { m_shader = shader; }

	void load() override;

	void destroy() override;

	void setTexture(int binding, str textureName);

	void update() {
		EtaBindings::updateBuffers(m_device);
		EtaBindings::updateTextures(m_device);
	}

	void setSharedBuffer(AllocatedBuffer& buffer, size_t offset) {
		m_sharedMaterialBuffer = buffer;
		m_bufferOffset = offset;
	}

protected:
	std::shared_ptr<EtaShader> m_shader;

	AllocatedBuffer m_sharedMaterialBuffer = {};
	size_t m_bufferOffset = 0; // TODO: AllocatedBuffer should have an offset field

	std::shared_ptr<EtaDescriptorAllocator> m_descriptorAllocator;
};

}; // namespace eta
