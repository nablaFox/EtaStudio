#pragma once

#include "../include/eta_system.hpp"
#include "../include/eta_scene.hpp"
#include "../include/eta_mesh.hpp"
#include "../include/eta_default_components.hpp"

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
		reserveVertices(34);
		reserveIndices(66);
	}
};

class PendulumSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update(float deltaTime) override {
		auto entities = currentScene().getEntities<MeshComponent, PendulumComponent, InputComponent>();
		auto camera = currentScene().getActiveCamera();

		if (camera == entt::null)
			return;

		auto& cameraTransform = currentScene().getComponent<TransformComponent>(camera);
		auto& cameraComponent = currentScene().getComponent<CameraComponent>(camera);

		entities.each([this, deltaTime, &cameraComponent, &cameraTransform](
						  auto entity, MeshComponent& mesh, PendulumComponent& pendulum, InputComponent& input) {
			if (input.keys[GLFW_KEY_UP]) {
				cameraComponent.orthoSize += 0.1f;
			}

			if (input.keys[GLFW_KEY_DOWN]) {
				cameraComponent.orthoSize -= 0.1f;
			}

			if (input.keys[GLFW_KEY_A]) {
				cameraTransform.position.x -= deltaTime;
			}

			if (input.keys[GLFW_KEY_D]) {
				cameraTransform.position.x += deltaTime;
			}

			if (input.keys[GLFW_KEY_W]) {
				cameraTransform.position.y += deltaTime;
			}

			if (input.keys[GLFW_KEY_S]) {
				cameraTransform.position.y -= deltaTime;
			}

			if (input.keys[GLFW_KEY_RIGHT]) {
				if (pendulum.angularVelocity < 0)
					pendulum.angularVelocity -= 0.1f;
				else
					pendulum.angularVelocity += 0.1f;
			}

			if (input.singleKeyPresses[GLFW_KEY_C]) {
				m_engine.switchScene("CubeScene");
			}

			const float gravity = 9.81f;
			const float damping = 0.99f;

			pendulum.angularAcceleration = -gravity / pendulum.length * glm::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angularVelocity *= damping;
			pendulum.angle += pendulum.angularVelocity * deltaTime;

			glm::vec3 bobPosition = pendulum.pivot + glm::vec3(pendulum.length * glm::sin(pendulum.angle),
															   pendulum.length * glm::cos(pendulum.angle), 0);

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

class PendulumScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		auto entity = addEntity();

		RenderComponent rendering = {.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP};
		addRenderComponent(entity, rendering);

		addMeshComponent(entity, "PendulumMesh");

		addComponent<PendulumComponent>(entity, 0.5f,  // length
										4.f,		   // angle
										0.10f,		   // angularVelocity
										0.4f,		   // angularAcceleration
										glm::vec3(0.0f, 0.0f, 0.0f));

		addComponent<InputComponent>(entity);

		addDefaultOrtographicCamera();
	};
};
