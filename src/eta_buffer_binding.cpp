#include "../include/eta_buffer_binding.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

size_t EtaBufferBinding::getSize() {
	size_t size = 0;

	size += sizeof(float) * data.floatProperties.size();
	size += sizeof(glm::vec2) * data.vec2Properties.size();
	size += sizeof(glm::vec3) * data.vec3Properties.size();
	size += sizeof(glm::vec4) * data.vec4Properties.size();
	size += sizeof(glm::mat4) * data.mat4Properties.size();

	return size;
}

void EtaBufferBinding::setProperty(const std::string& name, float value) {
	data.floatProperties[name] = value;
	m_dirty = true;
}

void EtaBufferBinding::setProperty(const std::string& name, const glm::vec2& value) {
	data.vec2Properties[name] = value;
	m_dirty = true;
}

void EtaBufferBinding::setProperty(const std::string& name, const glm::vec3& value) {
	data.vec3Properties[name] = value;
	m_dirty = true;
}

void EtaBufferBinding::setProperty(const std::string& name, const glm::vec4& value) {
	data.vec4Properties[name] = value;
	m_dirty = true;
}

void EtaBufferBinding::setProperty(const std::string& name, const glm::mat4& value) {
	data.mat4Properties[name] = value;
	m_dirty = true;
}

void EtaBufferBinding::updateBuffer(EtaDevice& device, AllocatedBuffer& buffer, EtaDescriptorSet& descriptorSet, size_t offset) {
	if (!m_dirty)
		return;

	if (buffer.buffer == VK_NULL_HANDLE && !offset)
		buffer = device.createUBO(getSize());

	descriptorSet.writeBuffer(0, buffer.buffer, getSize(), offset, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

	m_uboBuffer = buffer;
	m_bufferOffset = offset;
	m_dirty = false;
}
