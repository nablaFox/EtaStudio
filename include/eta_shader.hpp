#pragma once

#include "eta_asset.hpp"
#include "eta_device.hpp"
#include <fstream>

namespace eta {

class EtaShader : public EtaAsset {
	friend class EtaRenderingSystem;

public:
	using EtaAsset::EtaAsset;

	void load() override {
		if (!loadShaderModuleFromFile(m_vertShaderPath, m_vertShaderModule))
			fmt::println("Failed to load vertex shader: {}", m_vertShaderPath);
		else
			fmt::println("Loaded vertex shader: {}", m_vertShaderPath);

		if (!loadShaderModuleFromFile(m_fragShaderPath, m_fragShaderModule))
			return fmt::println("Failed to load fragment shader: {}", m_fragShaderPath);
		else
			fmt::println("Loaded fragment shader: {}", m_fragShaderPath);
	}

	bool loadShaderModuleFromFile(std::string filePath, VkShaderModule& shaderModule) {
		std::ifstream shaderFile(filePath, std::ios::ate | std::ios::binary);

		if (!shaderFile.is_open())
			return false;

		size_t fileSize = (size_t)shaderFile.tellg();

		std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

		shaderFile.seekg(0);

		shaderFile.read((char*)buffer.data(), fileSize);

		shaderFile.close();

		if (m_device.createShaderModule(buffer, shaderModule) != VK_SUCCESS)
			return false;

		return true;
	}

	void destroy() override {
		m_device.destroyShaderModule(m_fragShaderModule);
		m_device.destroyShaderModule(m_vertShaderModule);
	}

	void setFragShader(std::string shaderPath) { m_fragShaderPath = shaderPath; }
	void setVertShader(std::string shaderPath) { m_vertShaderPath = shaderPath; }

private:
	VkShaderModule m_fragShaderModule;
	VkShaderModule m_vertShaderModule;

	std::string m_fragShaderPath;
	std::string m_vertShaderPath;
};

} // namespace eta
