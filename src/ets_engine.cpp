#include "../include/ets_engine.hpp"

#include "../include/ets_rendering_system.hpp"
#include "../include/ets_initial_scene.hpp"

using namespace ets;

void EtaEngine::init(const GameEngineSettings& settings) {
	VK_CHECK(m_window.init(settings.appName, settings.windowWidth, settings.windowHeight));

	VK_CHECK(m_renderer.init(m_window));

	registerSystem<EtaRenderingSystem>();

	registerAsset<EtaInitialScene>("InitialScene");

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

		VK_CHECK(m_renderer.drawFrame());

		// TEMP: systems should have an update order
		for (auto& system : m_systems)
			system->update();

		VK_CHECK(m_renderer.endFrame());
	}
}

void EtaEngine::destroy() {
	m_renderer.destroy();
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

void EtaEngine::initTexture(const std::shared_ptr<EtaTextureAsset>& texture) {
	fmt::print("Texture '{}' initialized\n", texture->getName());
}

void EtaEngine::initMesh(const std::shared_ptr<EtaMeshAsset>& mesh) { fmt::print("Mesh '{}' initialized\n", mesh->getName()); }

void EtaEngine::initModel(const std::shared_ptr<EtaModelAsset>& model) {
	fmt::print("Model '{}' initialized\n", model->getName());
}

void EtaEngine::loadModelIntoScene(str modelName, str sceneName) {

	fmt::print("Model '{}' loaded into scene '{}'\n", modelName, sceneName);
}
