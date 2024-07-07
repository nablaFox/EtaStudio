#pragma once

#include "eta_pch.hpp"

namespace eta {

class EtaDevice;
class EtaDescriptorSet;

class EtaBufferBinding {
	friend class EtaBindings;

public:
	struct MaterialData {
		std::unordered_map<std::string, float> floatProperties;
		std::unordered_map<std::string, glm::vec2> vec2Properties;
		std::unordered_map<std::string, glm::vec3> vec3Properties;
		std::unordered_map<std::string, glm::vec4> vec4Properties;
		std::unordered_map<std::string, glm::mat4> mat4Properties;
	};

	size_t getSize();

	void setProperty(const std::string& name, float value);
	void setProperty(const std::string& name, const glm::vec2& value);
	void setProperty(const std::string& name, const glm::vec3& value);
	void setProperty(const std::string& name, const glm::vec4& value);
	void setProperty(const std::string& name, const glm::mat4& value);

	void updateBuffer(EtaDevice& device, AllocatedBuffer& buffer, EtaDescriptorSet& descriptorSet, size_t offset);
	void updateBuffer(EtaDevice& device, EtaDescriptorSet& descriptorSet) { updateBuffer(device, m_uboBuffer, descriptorSet, 0); }

private:
	bool m_dirty = false;
	size_t m_bufferOffset;
	AllocatedBuffer m_uboBuffer;
	MaterialData data;
};

}; // namespace eta
