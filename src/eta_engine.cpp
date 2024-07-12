#include "../include/eta_engine.hpp"

#include "../include/eta_rendering_system.hpp"
#include "../include/eta_input_system.hpp"
#include "../include/eta_initial_scene.hpp"
#include "../include/eta_default_materials.hpp"

using namespace eta;

void EtaEngine::init(const GameEngineSettings& settings) {
	ETA_CHECK(m_window.init(settings.appName, settings.windowWidth, settings.windowHeight));

	ETA_CHECK(m_device.init(m_window));

	registerAsset<EtaMetallicMaterial>("default_metallic");

	registerAsset<EtaInitialScene>("InitialScene");

	registerSystem<EtaInputSystem>(m_window);

	registerSystem<EtaRenderingSystem>(m_window, m_device);

	switchScene("InitialScene");

	fmt::print("Engine initialized\n");
}

void EtaEngine::run() {
	auto lastTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = 0.0f;

	while (!quit) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		m_deltaTime = deltaTime.count();

		// TEMP: systems should have an update order
		for (auto& system : m_systems)
			system->update(m_deltaTime);
	}
}

void EtaEngine::destroy() {
	for (auto& system : m_systems)
		system->sleep();

	m_device.waitIdle();

	m_assetManager.destroy();
	m_device.destroy();
	m_window.destroy();
}

void EtaEngine::switchScene(str name) {
	if (m_currentScene && m_currentScene->getName() == name) {
		fmt::print("Already in scene '{}'\n", name);
		return;
	}

	auto scene = m_assetManager.getAsset<EtaScene>(name);

	if (!scene) {
		fmt::print("Scene '{}' not found\n", name);
		return;
	}

	m_currentScene = scene;

	// reset keys
	m_systems[0]->update(0.0f);

	fmt::print("Switched to scene '{}'\n", name);
}

void EtaEngine::loadModelIntoScene(str modelName, str sceneName) {

	fmt::print("Model '{}' loaded into scene '{}'\n", modelName, sceneName);
}
