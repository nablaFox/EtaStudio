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

	void load() override;

	void destroy() override;

	void setTexture(int binding, str textureName);

	void update() { EtaBindings::updateBuffers(m_device); }

private:
	std::shared_ptr<EtaShader> m_shader;

	AllocatedBuffer m_uboBuffer = {};
	size_t m_bufferOffset = 0;

	std::shared_ptr<EtaDescriptorAllocator> m_descriptorAllocator;
};

}; // namespace eta
