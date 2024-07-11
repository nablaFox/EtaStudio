#include "../include/eta_app.hpp"

using namespace eta;

class DemoMesh : public EtaMeshAsset {
public:
	using EtaMeshAsset::EtaMeshAsset;

	void setup() override {
		std::vector<Vertex> rect_vertices(4);

		rect_vertices[0].position = {0.5, -0.5, 0};
		rect_vertices[1].position = {0.5, 0.5, 0};
		rect_vertices[2].position = {-0.5, -0.5, 0};
		rect_vertices[3].position = {-0.5, 0.5, 0};

		rect_vertices[0].color = {0, 0, 0, 1};
		rect_vertices[1].color = {0.5, 0.5, 0.5, 1};
		rect_vertices[2].color = {1, 0, 0, 1};
		rect_vertices[3].color = {0, 1, 0, 1};

		std::vector<uint32_t> rect_indices = {0, 1, 2, 2, 1, 3};

		setVertices(rect_vertices);
		setIndices(rect_indices);
	};
};

class DemoScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		fmt::println("DemoScene setup");

		auto entity = addEntity();

		addRenderComponent(entity);
		addMeshComponent(entity, "DemoMesh");

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
