#pragma once

#include "eta_pch.hpp"

#include "eta_mesh.hpp"
#include "eta_texture.hpp"
#include "eta_model.hpp"
#include "eta_scene.hpp"
#include "eta_shader.hpp"
#include "eta_material.hpp"

namespace eta {

class EtaAssetManager {
public:
	EtaAssetManager(EtaDevice& device) : m_device(device) {}

	template <typename T>
	std::shared_ptr<T> addAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");

		auto asset = std::make_shared<T>(m_device, *this, name);

		if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			m_meshAssets[name] = std::static_pointer_cast<EtaMeshAsset>(asset);
		else if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			m_textureAssets[name] = std::static_pointer_cast<EtaTextureAsset>(asset);
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			m_modelAssets[name] = std::static_pointer_cast<EtaModelAsset>(asset);
		else if constexpr (std::is_base_of<EtaSceneAsset, T>::value)
			m_sceneAssets[name] = std::static_pointer_cast<EtaSceneAsset>(asset);
		else if constexpr (std::is_base_of<EtaShader, T>::value)
			m_shaderAssets[name] = std::static_pointer_cast<EtaShader>(asset);
		else if constexpr (std::is_base_of<EtaMaterial, T>::value)
			m_materialAssets[name] = std::static_pointer_cast<EtaMaterial>(asset);

		return asset;
	}

	template <typename T>
	std::shared_ptr<T> getAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");

		if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			return m_meshAssets[name];
		else if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			return m_textureAssets[name];
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			return m_modelAssets[name];
		else if constexpr (std::is_base_of<EtaSceneAsset, T>::value)
			return m_sceneAssets[name];
		else if constexpr (std::is_base_of<EtaShader, T>::value)
			return m_shaderAssets[name];
		else if constexpr (std::is_base_of<EtaMaterial, T>::value)
			return m_materialAssets[name];

		return nullptr;
	}

	template <typename T>
	void removeAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");

		if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			m_meshAssets.erase(name);
		else if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			m_textureAssets.erase(name);
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			m_modelAssets.erase(name);
		else if constexpr (std::is_base_of<EtaSceneAsset, T>::value)
			m_sceneAssets.erase(name);
		else if constexpr (std::is_base_of<EtaShader, T>::value)
			m_shaderAssets.erase(name);
		else if constexpr (std::is_base_of<EtaMaterial, T>::value)
			m_materialAssets.erase(name);
	}

	EtaDevice& getDevice() { return m_device; }

private:
	std::unordered_map<std::string, std::shared_ptr<EtaMeshAsset>> m_meshAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaTextureAsset>> m_textureAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaModelAsset>> m_modelAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaSceneAsset>> m_sceneAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaShader>> m_shaderAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaMaterial>> m_materialAssets;

	EtaDevice& m_device;
};

}; // namespace eta