#include "../include/ets_engine.hpp"

using namespace ets;

struct DemoComponent {
	int value;
};

class DemoMesh : public EtaMeshAsset {
public:
	void setup() override {
		fmt::println("DemoMesh setup");
		//
	};
};

class DemoScene : public EtaSceneAsset {
public:
	void setup() override {
		fmt::println("DemoScene setup");

		addEntity<DemoComponent>();
	};
};

class DemoSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update() override {
		auto& scene = m_engine.currentScene();

		scene.getEntities<DemoComponent>().each([this](auto& component) {
			fmt::println("DemoComponent value: {}", component.value);

			component.value++;

			if (component.value % 10 == 0) {
				m_engine.switchScene("InitialScene");
			}
		});

		scene.getEntities<TransformComponent>().each([this](auto& transform) {
			transform.position.x += 1;

			if ((int)transform.position.x % 10 == 0) {
				m_engine.switchScene("DemoScene");
			}
		});

		// something
	};
};

class DemoApplication : public EtaApp {
	void setup() override {
		registerAsset<DemoMesh>("DemoMesh");
		registerAsset<DemoScene>("DemoScene");

		registerSystem<DemoSystem>();
	};
};

int main(int argc, char* argv[]) {
	DemoApplication app;

	app.init();

	app.run();

	app.destroy();

	return 0;
}
