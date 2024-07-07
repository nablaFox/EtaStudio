#pragma once

#include "eta_bindings.hpp"
#include "eta_asset.hpp"
#include "eta_buffer_binding.hpp"

namespace eta {
class EtaShader;

class EtaMaterial : public EtaAsset, public EtaBindings {
	using EtaAsset::EtaAsset;

public:
	void setFragShader(str shaderPath);

	void setVertShader(str shaderPath);

	void setProperty(str name, glm::vec4 value);

	void setProperty(str name, glm::vec3 value);

	void setProperty(str name, glm::vec2 value);

	void setProperty(str name, float value);

	void setProperty(str name, glm::mat4 value);

	void load() override;

	// TEMP: should call this only on setup
	void setTexture(uint32_t binding, str textureName);

private:
	EtaBufferBinding m_bufferBinding;
	std::shared_ptr<EtaShader> m_fragShader;
	std::shared_ptr<EtaShader> m_vertShader;

	AllocatedBuffer m_uboBuffer = {};
	size_t m_bufferOffset = 0;

	std::shared_ptr<EtaDescriptorAllocator> m_descriptorAllocator;
};

}; // namespace eta
