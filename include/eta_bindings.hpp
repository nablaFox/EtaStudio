#pragma once

#include "eta_pch.hpp"
#include "eta_descriptor.hpp"

namespace eta {

class EtaTextureAsset;
class EtaDevice;

class EtaBindings {
	struct EtaBufferData {
		std::unordered_map<std::string, float> floatProperties;
		std::unordered_map<std::string, glm::vec2> vec2Properties;
		std::unordered_map<std::string, glm::vec3> vec3Properties;
		std::unordered_map<std::string, glm::vec4> vec4Properties;
		std::unordered_map<std::string, glm::mat4> mat4Properties;
	};

	struct EtaBufferBinding {
		EtaBufferData data;
		AllocatedBuffer m_uboBuffer = {};
		size_t offset = 0;
		size_t size = 0;
		bool m_dirty = false;
	};

public:
	VkResult init(EtaDevice& device, EtaDescriptorAllocator& descriptorAllocator);
	void destroy(EtaDevice& device);

	void addBufferBinding(int binding, AllocatedBuffer& buffer, size_t offset);

	void addBufferBinding(int binding);

	void updateBuffers(EtaDevice& device);

	void addFloat(int binding, const std::string& name, float value);
	void addVec2(int binding, const std::string& name, const glm::vec2& value);
	void addVec3(int binding, const std::string& name, const glm::vec3& value);
	void addVec4(int binding, const std::string& name, const glm::vec4& value);
	void addMat4(int binding, const std::string& name, const glm::mat4& value);

	void setFloat(int binding, const std::string& name, float value);
	void setVec2(int binding, const std::string& name, const glm::vec2& value);
	void setVec3(int binding, const std::string& name, const glm::vec3& value);
	void setVec4(int binding, const std::string& name, const glm::vec4& value);
	void setMat4(int binding, const std::string& name, const glm::mat4& value);

	void addTextureBinding(int binding, std::shared_ptr<EtaTextureAsset> texture);

	size_t getHash();

	VkDescriptorSet getDescriptorSet() { return m_descriptorSet.get(); }
	VkDescriptorSetLayout getDescriptorSetLayout() { return m_descriptorSetLayout.get(); }

protected:
	std::unordered_map<int, std::shared_ptr<EtaTextureAsset>> m_textureBindings;
	std::unordered_map<int, std::shared_ptr<EtaBufferBinding>> m_bufferBindings;
	EtaDescriptorSet m_descriptorSet;
	EtaDescriptorLayout m_descriptorSetLayout;

private:
	void addBufferBinding(int binding, std::shared_ptr<EtaBufferBinding> bufferBinding);

	template <typename T>
	void setBufferProperty(int binding, const std::string& name, const T& value,
						   std::unordered_map<std::string, T>& propertyMap) {
		auto bufferBinding = m_bufferBindings[binding];
		propertyMap[name] = value;
		bufferBinding->size += sizeof(T);
		bufferBinding->m_dirty = true;
	}

	template <typename T>
	void updateBufferProperty(int binding, const std::string& name, const T& value,
							  std::unordered_map<std::string, T>& propertyMap) {
		auto bufferBinding = m_bufferBindings[binding];
		if (propertyMap.find(name) == propertyMap.end())
			return;
		propertyMap[name] = value;
		bufferBinding->m_dirty = true;
	}
};

} // namespace eta
