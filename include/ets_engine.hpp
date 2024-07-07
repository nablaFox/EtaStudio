#pragma once

#include "ets_pch.hpp"
#include "ets_system.hpp"
#include "ets_texture.hpp"
#include "ets_mesh.hpp"
#include "ets_model.hpp"
#include "ets_scene.hpp"

#include "ets_asset_manager.hpp"

#include "ets_renderer.hpp"

#include "ets_window.hpp"

namespace ets {

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

	std::shared_ptr<EtaSceneAsset> createScene(str name) { return m_assetManager.addAsset<EtaSceneAsset>(name); }
	EtaSceneAsset& currentScene() { return *m_currentScene; }
	void destroyScene(str name) { m_assetManager.removeAsset<EtaSceneAsset>(name); }
	void switchScene(str name);

	std::shared_ptr<EtaTextureAsset> createTexture(str name) { return m_assetManager.addAsset<EtaTextureAsset>(name); }
	void initTexture(const std::shared_ptr<EtaTextureAsset>& texture);

	std::shared_ptr<EtaMeshAsset> createMesh(str name) { return m_assetManager.addAsset<EtaMeshAsset>(name); }
	void initMesh(const std::shared_ptr<EtaMeshAsset>& mesh);

	std::shared_ptr<EtaModelAsset> createModel(str name) { return m_assetManager.addAsset<EtaModelAsset>(name); }
	void initModel(const std::shared_ptr<EtaModelAsset>& model);

	void loadModelIntoScene(str modelName, str sceneName);

protected:
	void init(const GameEngineSettings& settings = defaultSettings);

	void run();

	void destroy();

	template <typename T>
	void registerAsset(str name) {
		static_assert(std::is_base_of<EtaAsset, T>::value, "T must derive from EtaAsset");

		std::shared_ptr<T> asset = m_assetManager.addAsset<T>(name);
		asset->setup();

		if constexpr (std::is_base_of<EtaTextureAsset, T>::value)
			initTexture(asset);
		else if constexpr (std::is_base_of<EtaMeshAsset, T>::value)
			initMesh(std::static_pointer_cast<EtaMeshAsset>(asset));
		else if constexpr (std::is_base_of<EtaModelAsset, T>::value)
			initModel(asset);
	};

	template <typename T>
	void registerSystem() {
		static_assert(std::is_base_of<EtaSystem, T>::value, "T must derive from EtaSystem");
		m_systems.push_back(std::make_shared<T>(*this));
	}

private:
	std::vector<std::shared_ptr<EtaSystem>> m_systems;
	EtaAssetManager m_assetManager;
	std::shared_ptr<EtaSceneAsset> m_currentScene;
	EtaRenderer m_renderer;
	EtaWindow m_window;
};

class EtaApp : public EtaEngine {
public:
	void init(const GameEngineSettings& settings = EtaEngine::defaultSettings) {
		EtaEngine::init(settings);
		setup();

		for (auto& system : m_systems)
			system->awake();
	}

	void run() { EtaEngine::run(); }

	void destroy() { EtaEngine::destroy(); }

	virtual void setup() = 0;

private:
};

}; // namespace ets
