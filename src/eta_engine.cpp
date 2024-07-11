#include "../include/eta_engine.hpp"

#include "../include/eta_rendering_system.hpp"
#include "../include/eta_initial_scene.hpp"
#include "../include/eta_default_materials.hpp"

using namespace eta;

void EtaEngine::init(const GameEngineSettings& settings) {
	ETA_CHECK(m_window.init(settings.appName, settings.windowWidth, settings.windowHeight));

	ETA_CHECK(m_device.init(m_window));

	registerAsset<EtaMetallicMaterial>("default_metallic");

	registerAsset<EtaInitialScene>("InitialScene");

	registerSystem<EtaRenderingSystem>(m_window, m_device);

	switchScene("InitialScene");

	fmt::print("Engine initialized\n");
}

void EtaEngine::run() {
	bool quit = false;

	// TEMP: events should be handled by the input system
	while (!quit) {
		glfwPollEvents();

		if (glfwWindowShouldClose(m_window.getWindow()))
			quit = true;

		// TEMP: systems should have an update order
		for (auto& system : m_systems)
			system->update();
	}
}

void EtaEngine::destroy() {
	for (auto& system : m_systems)
		system->sleep();

	m_assetManager.destroy();
	m_device.destroy();
	m_window.destroy();
}

void EtaEngine::switchScene(str name) {
	if (m_currentScene && m_currentScene->getName() == name) {
		fmt::print("Already in scene '{}'\n", name);
		return;
	}

	auto scene = m_assetManager.getAsset<EtaSceneAsset>(name);

	if (!scene) {
		fmt::print("Scene '{}' not found\n", name);
		return;
	}

	m_currentScene = scene;
	fmt::print("Switched to scene '{}'\n", name);
}

void EtaEngine::loadModelIntoScene(str modelName, str sceneName) {

	fmt::print("Model '{}' loaded into scene '{}'\n", modelName, sceneName);
}
