#pragma once

#include "eta_pch.hpp"
#include "eta_device.hpp"

#include "eta_asset_manager.hpp"

#include "eta_window.hpp"

namespace eta {

class EtaSystem;

struct GameEngineSettings {
	const char* appName = "Eta Engine";
	uint32_t windowWidth = 1280;
	uint32_t windowHeight = 720;
	bool resizableWindow = false;
};

class EtaEngine {
	friend class EtaApp;

public:
	static constexpr GameEngineSettings defaultSettings{};

	std::shared_ptr<EtaScene> createScene(str name) { return m_assetManager.addAsset<EtaScene>(name); }
	EtaScene& currentScene() { return *m_currentScene; }
	void destroyScene(str name) { m_assetManager.removeAsset<EtaScene>(name); }
	void switchScene(str name);

	std::shared_ptr<EtaTextureAsset> createTexture(str name) { return m_assetManager.addAsset<EtaTextureAsset>(name); }
	std::shared_ptr<EtaTextureAsset> getTexture(str name) { return m_assetManager.getAsset<EtaTextureAsset>(name); }

	std::shared_ptr<EtaMeshAsset> createMesh(str name) { return m_assetManager.addAsset<EtaMeshAsset>(name); }
	std::shared_ptr<EtaMeshAsset> getMesh(str name) { return m_assetManager.getAsset<EtaMeshAsset>(name); }

	std::shared_ptr<EtaModelAsset> createModel(str name) { return m_assetManager.addAsset<EtaModelAsset>(name); }
	std::shared_ptr<EtaModelAsset> getModel(str name) { return m_assetManager.getAsset<EtaModelAsset>(name); }

	std::shared_ptr<EtaShader> createShader(str name) { return m_assetManager.addAsset<EtaShader>(name); }
	std::shared_ptr<EtaShader> getShader(str name) { return m_assetManager.getAsset<EtaShader>(name); }

	std::shared_ptr<EtaMaterial> createMaterial(str name) { return m_assetManager.addAsset<EtaMaterial>(name); }
	std::shared_ptr<EtaMaterial> getMaterial(str name) { return m_assetManager.getAsset<EtaMaterial>(name); }

	void loadModelIntoScene(str modelName, str sceneName);

protected:
	void init(const GameEngineSettings& settings = defaultSettings);

	void run();

	void destroy();

	template <typename T>
	void registerAsset(str name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaAsset");
		auto asset = m_assetManager.addAsset<T>(name);
		asset->setup();
		asset->load();
	};

	template <typename T, typename... Args>
	void registerSystem(Args&&... args) {
		static_assert(std::is_base_of<EtaSystem, T>::value, "T must derive from EtaSystem");
		m_systems.push_back(std::make_shared<T>(*this, std::forward<Args>(args)...));
	}

private:
	std::vector<std::shared_ptr<EtaSystem>> m_systems;
	std::shared_ptr<EtaScene> m_currentScene;
	EtaWindow m_window;
	EtaDevice m_device;
	EtaAssetManager m_assetManager{m_device};
};

}; // namespace eta
