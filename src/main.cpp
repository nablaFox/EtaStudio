#include "../include/eta_app.hpp"

using namespace eta;

struct PendulumComponent {
	float length;
	float angle;
	float angularVelocity;
	float angularAcceleration;
	glm::vec3 pivot;
};

class PendulumMesh : public EtaMeshAsset {
	using EtaMeshAsset::EtaMeshAsset;

public:
	void setup() override {
		reserveVertices(34); // 2 for the line and 32 for the circle
		reserveIndices(66);
	}
};

class PendulumSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update(float deltaTime) override {
		auto entities = currentScene().getEntities<TransformComponent, MeshComponent, PendulumComponent>();

		entities.each(
			[this, deltaTime](auto entity, TransformComponent& transform, MeshComponent& mesh, PendulumComponent& pendulum) {
				const float gravity = 9.81f;
				const float damping = 0.99f;

				pendulum.angularAcceleration = -gravity / pendulum.length * glm::sin(pendulum.angle);
				pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
				pendulum.angularVelocity *= damping;
				pendulum.angle += pendulum.angularVelocity * deltaTime;

				glm::vec3 bobPosition = pendulum.pivot + glm::vec3(pendulum.length * glm::sin(pendulum.angle),
																   pendulum.length * glm::cos(pendulum.angle), 0);

				transform.position = bobPosition;

				const int circleSegments = 32;
				const float radius = 0.1f;

				std::vector<Vertex> vertices(2 + circleSegments);
				std::vector<uint32_t> indices(2 + 2 * circleSegments);

				vertices[0].position = pendulum.pivot;
				vertices[0].color = {0.5, 0.5, 0.5, 1};
				vertices[1].position = bobPosition;
				vertices[1].color = {0.5, 0.5, 0.5, 1};

				indices[0] = 0;
				indices[1] = 1;

				static constexpr float pi = 3.14159265359;

				// Circle vertices and indices
				for (int i = 0; i < circleSegments; ++i) {
					float theta = 2.0f * pi * float(i) / float(circleSegments);

					glm::vec3 circlePos = bobPosition + glm::vec3(radius * glm::cos(theta), radius * glm::sin(theta), 0.0f);
					vertices[2 + i].position = circlePos;
					vertices[2 + i].color = {1.0, 0.0, 0.0, 1};

					indices[2 + 2 * i] = 1 + i;
					indices[3 + 2 * i] = 2 + ((i + 1) % circleSegments);
				}

				mesh.meshAsset->setVertices(vertices);
				mesh.meshAsset->setIndices(indices);
			});
	}
};

class DemoScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		fmt::println("DemoScene setup");

		auto entity = addEntity();

		addRenderComponent(entity);
		addMeshComponent(entity, "PendulumMesh");
		addComponent<PendulumComponent>(entity, 0.5f, // length
										4.f,		  // angle
										0.10f,		  // angularVelocity
										0.4f,		  // angularAcceleration
										glm::vec3(0.0f, 0.0f, 0.0f));

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
		registerAsset<PendulumMesh>("PendulumMesh");
		registerAsset<DemoScene>("DemoScene");
		registerSystem<PendulumSystem>();

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
