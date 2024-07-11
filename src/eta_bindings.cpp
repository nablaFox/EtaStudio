#include "../include/eta_bindings.hpp"
#include "../include/eta_device.hpp"
#include "../include/eta_texture.hpp"

using namespace eta;

void EtaBindings::setBufferProperty(int binding, const std::string& name, float value) {
	auto bufferBinding = m_bufferBindings[binding];
	bufferBinding->data.floatProperties[name] = value;
	bufferBinding->size += sizeof(float);
	bufferBinding->m_dirty = true;
}

void EtaBindings::setBufferProperty(int binding, const std::string& name, const glm::vec2& value) {
	auto bufferBinding = m_bufferBindings[binding];
	bufferBinding->data.vec2Properties[name] = value;
	bufferBinding->size += sizeof(glm::vec2);
	bufferBinding->m_dirty = true;
}

void EtaBindings::setBufferProperty(int binding, const std::string& name, const glm::vec3& value) {
	auto bufferBinding = m_bufferBindings[binding];
	bufferBinding->data.vec3Properties[name] = value;
	bufferBinding->size += sizeof(glm::vec3);
	bufferBinding->m_dirty = true;
}

void EtaBindings::setBufferProperty(int binding, const std::string& name, const glm::vec4& value) {
	auto bufferBinding = m_bufferBindings[binding];
	bufferBinding->data.vec4Properties[name] = value;
	bufferBinding->size += sizeof(glm::vec4);
	bufferBinding->m_dirty = true;
}

void EtaBindings::setBufferProperty(int binding, const std::string& name, const glm::mat4& value) {
	auto bufferBinding = m_bufferBindings[binding];
	bufferBinding->data.mat4Properties[name] = value;
	bufferBinding->size += sizeof(glm::mat4);
	bufferBinding->m_dirty = true;
}

void EtaBindings::addBufferBinding(int binding) {
	auto bufferBinding = std::make_shared<EtaBufferBinding>();
	addBufferBinding(binding, bufferBinding);
}

void EtaBindings::addBufferBinding(int binding, AllocatedBuffer& buffer, size_t offset) {
	auto bufferBinding = std::make_shared<EtaBufferBinding>();
	bufferBinding->offset = offset;
	bufferBinding->m_uboBuffer = buffer;
	addBufferBinding(binding, bufferBinding);
}

void EtaBindings::addBufferBinding(int binding, std::shared_ptr<EtaBufferBinding> bufferBinding) {
	m_bufferBindings[binding] = bufferBinding;
	m_descriptorSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
									 VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT);
}

void EtaBindings::addTextureBinding(int binding, std::shared_ptr<EtaTextureAsset> texture) {
	m_descriptorSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
	m_textureBindings[binding] = texture;
}

VkResult EtaBindings::init(EtaDevice& device, EtaDescriptorAllocator& descriptorAllocator) {
	VK_RETURN(m_descriptorSetLayout.allocate(device));

	VK_RETURN(descriptorAllocator.allocate(device, m_descriptorSetLayout, m_descriptorSet));

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		if (bufferBinding->m_uboBuffer.buffer == VK_NULL_HANDLE)
			device.createUBO(bufferBinding->size, bufferBinding->m_uboBuffer);

		m_descriptorSet.writeBuffer(binding, bufferBinding->m_uboBuffer.buffer, bufferBinding->size, bufferBinding->offset,
									VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	}

	for (auto& [binding, texture] : m_textureBindings) {
		m_descriptorSet.writeImage(binding, texture->getImage().imageView, texture->getSampler(),
								   texture->getImage().currentLayout);
	}

	m_descriptorSet.update(device);
	return VK_SUCCESS;
}

void EtaBindings::updateBuffers(EtaDevice& device) {
	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		if (!bufferBinding->m_dirty)
			continue;

		ETA_CHECK_MSG(
			device.fillBuffer(bufferBinding->m_uboBuffer, &bufferBinding->data, bufferBinding->size, bufferBinding->offset),
			"Failed to fill buffer");
	}
}

size_t EtaBindings::getHash() {
	std::string bufferLayout;

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		bufferLayout += std::to_string(binding) + ":";
		bufferLayout += std::to_string(bufferBinding->data.floatProperties.size()) + "f,";
		bufferLayout += std::to_string(bufferBinding->data.vec2Properties.size()) + "v2,";
		bufferLayout += std::to_string(bufferBinding->data.vec3Properties.size()) + "v3,";
		bufferLayout += std::to_string(bufferBinding->data.vec4Properties.size()) + "v4,";
		bufferLayout += std::to_string(bufferBinding->data.mat4Properties.size()) + "m4;";
	}

	std::string textureLayout = std::to_string(m_textureBindings.size());

	return std::hash<std::string>{}(bufferLayout + textureLayout);
}

void EtaBindings::destroy(EtaDevice& device) {
	m_descriptorSetLayout.destroy(device);

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		device.destroyBuffer(bufferBinding->m_uboBuffer);
	}
}
