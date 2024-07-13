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

		auto& assetsMap = getAssetsMap<T>();
		auto asset = std::make_shared<T>(m_device, *this, name);
		assetsMap[name] = asset;

		return asset;
	}

	template <typename T>
	std::shared_ptr<T> getAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");

		auto asset = getAssetsMap<T>()[name];
		if (!asset)
			fmt::print("Warning: {} {} not found\n", getAssetTypeName<T>(), name);

		return std::static_pointer_cast<T>(asset);
	}

	template <typename T>
	void removeAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");
		getAssetsMap<T>().erase(name);
	}

	void destroy() {
		destroyAssets(m_meshAssets, "mesh");
		destroyAssets(m_textureAssets, "texture");
		destroyAssets(m_modelAssets, "model");
		destroyAssets(m_sceneAssets, "scene");
		destroyAssets(m_shaderAssets, "shader");
		destroyAssets(m_materialAssets, "material");

		clearAllAssets();
	}

	template <typename T>
	std::string getAssetTypeName() {
		if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			return "Mesh";
		else if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			return "Texture";
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			return "Model";
		else if constexpr (std::is_base_of<EtaScene, T>::value)
			return "Scene";
		else if constexpr (std::is_base_of<EtaShader, T>::value)
			return "Shader";
		else if constexpr (std::is_base_of<EtaMaterial, T>::value)
			return "Material";
		else
			return "Unknown Asset Type";
	}

private:
	template <typename T>
	auto& getAssetsMap() {
		if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			return m_meshAssets;
		else if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			return m_textureAssets;
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			return m_modelAssets;
		else if constexpr (std::is_base_of<EtaScene, T>::value)
			return m_sceneAssets;
		else if constexpr (std::is_base_of<EtaShader, T>::value)
			return m_shaderAssets;
		else if constexpr (std::is_base_of<EtaMaterial, T>::value)
			return m_materialAssets;
	}

	template <typename MapType>
	void destroyAssets(MapType& assets, const std::string& assetType) {
		for (auto& [name, asset] : assets) {
			fmt::print("Info: Destroying {} {}\n", assetType, name);
			if (asset)
				asset->destroy();
		}
		assets.clear();
	}

	void clearAllAssets() {
		m_meshAssets.clear();
		m_textureAssets.clear();
		m_modelAssets.clear();
		m_sceneAssets.clear();
		m_shaderAssets.clear();
		m_materialAssets.clear();
	}

	std::unordered_map<std::string, std::shared_ptr<EtaMeshAsset>> m_meshAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaTextureAsset>> m_textureAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaModelAsset>> m_modelAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaScene>> m_sceneAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaShader>> m_shaderAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaMaterial>> m_materialAssets;

	EtaDevice& m_device;
};

}; // namespace eta
