#include "../include/eta_bindings.hpp"
#include "../include/eta_device.hpp"
#include "../include/eta_texture.hpp"

using namespace eta;

void EtaBindings::addFloat(int binding, const std::string& name, float value) {
	setBufferProperty(binding, name, value, m_bufferBindings[binding].data.floatProperties);
}

void EtaBindings::addVec2(int binding, const std::string& name, const glm::vec2& value) {
	setBufferProperty(binding, name, value, m_bufferBindings[binding].data.vec2Properties);
}

void EtaBindings::addVec3(int binding, const std::string& name, const glm::vec3& value) {
	setBufferProperty(binding, name, value, m_bufferBindings[binding].data.vec3Properties);
}

void EtaBindings::addVec4(int binding, const std::string& name, const glm::vec4& value) {
	setBufferProperty(binding, name, value, m_bufferBindings[binding].data.vec4Properties);
}

void EtaBindings::addMat4(int binding, const std::string& name, const glm::mat4& value) {
	setBufferProperty(binding, name, value, m_bufferBindings[binding].data.mat4Properties);
}

void EtaBindings::setFloat(int binding, const std::string& name, float value) {
	updateBufferProperty(binding, name, value, m_bufferBindings[binding].data.floatProperties);
}

void EtaBindings::setVec2(int binding, const std::string& name, const glm::vec2& value) {
	updateBufferProperty(binding, name, value, m_bufferBindings[binding].data.vec2Properties);
}

void EtaBindings::setVec3(int binding, const std::string& name, const glm::vec3& value) {
	updateBufferProperty(binding, name, value, m_bufferBindings[binding].data.vec3Properties);
}

void EtaBindings::setVec4(int binding, const std::string& name, const glm::vec4& value) {
	updateBufferProperty(binding, name, value, m_bufferBindings[binding].data.vec4Properties);
}

void EtaBindings::setMat4(int binding, const std::string& name, const glm::mat4& value) {
	updateBufferProperty(binding, name, value, m_bufferBindings[binding].data.mat4Properties);
}

void EtaBindings::addBufferBinding(int binding) {
	EtaBufferBinding bufferBinding;
	addBufferBinding(binding, bufferBinding);
}

void EtaBindings::addBufferBinding(int binding, AllocatedBuffer& buffer, size_t offset) {
	EtaBufferBinding bufferBinding;
	bufferBinding.offset = offset;
	bufferBinding.m_uboBuffer = buffer;
	bufferBinding.shared = true;
	addBufferBinding(binding, bufferBinding);
}

void EtaBindings::addBufferBinding(int binding, EtaBufferBinding& bufferBinding) {
	m_bufferBindings[binding] = bufferBinding;
	m_descriptorSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
									 VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT);
}

void EtaBindings::setTextureBinding(int binding, std::shared_ptr<EtaTextureAsset> texture) {
	if (m_textureBindings.find(binding) != m_textureBindings.end()) {
		m_textureBindings[binding].texture = texture;
		m_textureBindings[binding].m_dirty = true;
		m_oneTextureDirty = true;
		return;
	}

	EtaTextureBinding textureBinding;
	textureBinding.m_dirty = true;
	textureBinding.texture = texture;
	m_textureBindings[binding] = textureBinding;

	m_descriptorSetLayout.addBinding(binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
}

VkResult EtaBindings::init(EtaDevice& device, EtaDescriptorAllocator& descriptorAllocator) {
	VK_RETURN(m_descriptorSetLayout.allocate(device));

	VK_RETURN(descriptorAllocator.allocate(device, m_descriptorSetLayout, m_descriptorSet));

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		if (bufferBinding.m_uboBuffer.buffer == VK_NULL_HANDLE)
			device.createUBO(bufferBinding.size, bufferBinding.m_uboBuffer);

		if (bufferBinding.m_uboBuffer.buffer == VK_NULL_HANDLE)
			return VK_ERROR_INITIALIZATION_FAILED;
	}

	writeBufferBindings();
	writeTextureBindings();
	m_descriptorSet.update(device);

	return VK_SUCCESS;
}

void EtaBindings::writeTextureBindings() {
	for (auto& [binding, textureBinding] : m_textureBindings) {
		if (textureBinding.m_dirty == false)
			return;

		auto texture = textureBinding.texture;
		m_descriptorSet.writeImage(binding, texture->getImage().imageView, texture->getSampler(),
								   texture->getImage().currentLayout, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
		textureBinding.m_dirty = false;
	}
}

void EtaBindings::writeBufferBindings() {
	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		m_descriptorSet.writeBuffer(binding, bufferBinding.m_uboBuffer.buffer, bufferBinding.size, bufferBinding.offset,
									VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	}
}

void EtaBindings::updateTextures(EtaDevice& device) {
	if (!m_oneTextureDirty)
		return;

	writeTextureBindings();
	m_descriptorSet.update(device);
	m_oneTextureDirty = false;
}

void EtaBindings::updateBuffers(EtaDevice& device) {
	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		if (!bufferBinding.m_dirty)
			continue;

		std::vector<uint8_t> alignedBuffer;
		alignedBuffer.reserve(bufferBinding.size);

		auto appendData = [&alignedBuffer](const auto& data, size_t dataSize) {
			alignedBuffer.insert(alignedBuffer.end(), reinterpret_cast<const uint8_t*>(&data),
								 reinterpret_cast<const uint8_t*>(&data) + dataSize);
		};

		for (const auto& [name, value] : bufferBinding.data.mat4Properties)
			appendData(value, sizeof(glm::mat4));

		for (const auto& [name, value] : bufferBinding.data.vec4Properties)
			appendData(value, sizeof(glm::vec4));

		for (const auto& [name, value] : bufferBinding.data.vec3Properties)
			appendData(value, sizeof(glm::vec3));

		for (const auto& [name, value] : bufferBinding.data.vec2Properties)
			appendData(value, sizeof(glm::vec2));

		for (const auto& [name, value] : bufferBinding.data.floatProperties)
			appendData(value, sizeof(float));

		ETA_CHECK_MSG(
			device.fillBuffer(bufferBinding.m_uboBuffer, alignedBuffer.data(), alignedBuffer.size(), bufferBinding.offset),
			"Failed to fill buffer");

		bufferBinding.m_dirty = false;
	}
}

size_t EtaBindings::getHash() {
	std::string bufferLayout;

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		bufferLayout += std::to_string(binding) + ":";
		bufferLayout += std::to_string(bufferBinding.data.floatProperties.size()) + "f,";
		bufferLayout += std::to_string(bufferBinding.data.vec2Properties.size()) + "v2,";
		bufferLayout += std::to_string(bufferBinding.data.vec3Properties.size()) + "v3,";
		bufferLayout += std::to_string(bufferBinding.data.vec4Properties.size()) + "v4,";
		bufferLayout += std::to_string(bufferBinding.data.mat4Properties.size()) + "m4;";
	}

	std::string textureLayout = std::to_string(m_textureBindings.size());

	return std::hash<std::string>{}(bufferLayout + textureLayout);
}

void EtaBindings::destroy(EtaDevice& device) {
	device.waitIdle();
	m_descriptorSetLayout.destroy(device);

	for (auto& [binding, bufferBinding] : m_bufferBindings) {
		if (bufferBinding.shared == false)
			device.destroyBuffer(bufferBinding.m_uboBuffer);
	}
}
