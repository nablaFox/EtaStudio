#include "../include/eta_app.hpp"

using namespace eta;

class DemoMesh : public EtaMeshAsset {
public:
	using EtaMeshAsset::EtaMeshAsset;

	void setup() override {
		setVertices({
			{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		});

		setIndices({0, 1, 2, 2, 3, 0});
	};
};

class DemoScene : public EtaSceneAsset {
public:
	using EtaSceneAsset::EtaSceneAsset;

	void setup() override {
		fmt::println("DemoScene setup");

		auto entity = addEntity();

		addComponent<MeshComponent>(entity, "DemoMesh");
		addComponent<RenderComponent>(entity, "default_metallic");

		addEntity<CameraComponent>(45.0f,  // fov
								   1.77f,  // aspect
								   0.1f,   // near
								   100.0f, // far
								   true,   // enabled
								   glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	};
};

class DemoApplication : public EtaApp {
	void setup() override {
		registerAsset<DemoMesh>("DemoMesh");
		registerAsset<DemoScene>("DemoScene");

		switchScene("DemoScene");
	};
};

int main(int argc, char* argv[]) {
	DemoApplication app;

	app.init();

	app.run();

	app.destroy();

	return 0;
}
