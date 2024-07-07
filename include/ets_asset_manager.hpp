#pragma once

#include "ets_pch.hpp"
#include "ets_asset.hpp"

namespace ets {

class EtaMeshAsset;
class EtaTextureAsset;
class EtaModelAsset;
class EtaSceneAsset;

class EtaAssetManager {
public:
	template <typename T>
	std::shared_ptr<T> addAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");

		auto asset = std::make_shared<T>();
		asset->name = name;

		if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			m_meshAssets[name] = std::static_pointer_cast<EtaMeshAsset>(asset);
		else if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			m_textureAssets[name] = std::static_pointer_cast<EtaTextureAsset>(asset);
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			m_modelAssets[name] = std::static_pointer_cast<EtaModelAsset>(asset);
		else if constexpr (std::is_base_of<EtaSceneAsset, T>::value)
			m_sceneAssets[name] = std::static_pointer_cast<EtaSceneAsset>(asset);

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

		return nullptr;
	}

	template <typename T>
	void removeAsset(const std::string& name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaBaseAsset");

		if constexpr (std::is_same<T, EtaMeshAsset>::value)
			m_meshAssets.erase(name);
		else if constexpr (std::is_same<T, EtaTextureAsset>::value)
			m_textureAssets.erase(name);
		else if constexpr (std::is_same<T, EtaModelAsset>::value)
			m_modelAssets.erase(name);
		else if constexpr (std::is_same<T, EtaSceneAsset>::value)
			m_sceneAssets.erase(name);
	}

private:
	std::unordered_map<std::string, std::shared_ptr<EtaMeshAsset>> m_meshAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaTextureAsset>> m_textureAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaModelAsset>> m_modelAssets;
	std::unordered_map<std::string, std::shared_ptr<EtaSceneAsset>> m_sceneAssets;
};

}; // namespace ets
