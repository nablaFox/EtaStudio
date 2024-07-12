#pragma once

#include "../include/eta_app.hpp"
#include <glm/gtc/quaternion.hpp>

using namespace eta;

struct CubeComponent {
	float rotationSpeed;
	glm::vec3 rotationAxis;
};

class CubeScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		fmt::println("CubeScene setup");

		auto cube = addEntity();

		addRenderComponent(cube);
		addMeshComponent(cube, "CubeMesh");

		addComponent<CubeComponent>(cube,						  // entity
									0.5f,						  // rotationSpeed
									glm::vec3(1.0f, 0.0f, 0.0f)); // rotationAxis

		addComponent<InputComponent>(cube);

		addDefaultCamera();
	};
};

class CubeMesh : public EtaMeshAsset {
	using EtaMeshAsset::EtaMeshAsset;

public:
	void setup() override {
		std::vector<Vertex> vertices(8);

		vertices[0].position = {-0.5f, -0.5f, -0.5f};
		vertices[1].position = {0.5f, -0.5f, -0.5f};
		vertices[2].position = {0.5f, 0.5f, -0.5f};
		vertices[3].position = {-0.5f, 0.5f, -0.5f};
		vertices[4].position = {-0.5f, -0.5f, 0.5f};
		vertices[5].position = {0.5f, -0.5f, 0.5f};
		vertices[6].position = {0.5f, 0.5f, 0.5f};
		vertices[7].position = {-0.5f, 0.5f, 0.5f};

		// setup uv coordinates
		vertices[0].uv_x = 0.0f;
		vertices[0].uv_y = 0.0f;
		vertices[1].uv_x = 1.0f;
		vertices[1].uv_y = 0.0f;
		vertices[2].uv_x = 1.0f;
		vertices[2].uv_y = 1.0f;
		vertices[3].uv_x = 0.0f;
		vertices[3].uv_y = 1.0f;
		vertices[4].uv_x = 0.0f;
		vertices[4].uv_y = 0.0f;
		vertices[5].uv_x = 1.0f;
		vertices[5].uv_y = 0.0f;
		vertices[6].uv_x = 1.0f;
		vertices[6].uv_y = 1.0f;
		vertices[7].uv_x = 0.0f;
		vertices[7].uv_y = 1.0f;

		for (auto& vertex : vertices) {
			vertex.color = {1.0, 0.0, 0.0, 1};
		}

		std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0, // back face
			4, 5, 6, 6, 7, 4, // front face
			0, 1, 5, 5, 4, 0, // bottom face
			2, 3, 7, 7, 6, 2, // top face
			0, 3, 7, 7, 4, 0, // left face
			1, 2, 6, 6, 5, 1  // right face
		};

		setVertices(vertices);
		setIndices(indices);
	}
};

class CubeSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update(float deltaTime) override {
		auto entities = currentScene().getEntities<TransformComponent, CubeComponent, InputComponent>();
		auto cameraTransform = currentScene().getActiveCameraTransform();

		entities.each([this, deltaTime, cameraTransform](auto entity, TransformComponent& transform, CubeComponent& cube,
														 InputComponent& input) {
			if (input.singleKeyPresses[GLFW_KEY_X]) {
				m_engine.switchScene("PendulumScene");
			}

			const float rotationSpeed = 30.0f; // degrees per second
			float yaw = 0.0f;
			float pitch = 0.0f;

			if (input.keys[GLFW_KEY_RIGHT]) {
				yaw -= rotationSpeed * deltaTime;
			}

			if (input.keys[GLFW_KEY_LEFT]) {
				yaw += rotationSpeed * deltaTime;
			}

			if (input.keys[GLFW_KEY_UP]) {
				pitch -= rotationSpeed * deltaTime;
			}

			if (input.keys[GLFW_KEY_DOWN]) {
				pitch += rotationSpeed * deltaTime;
			}

			if (input.keys[GLFW_KEY_A]) {
				cameraTransform->position.x -= cameraSpeed * deltaTime * 10;
			}

			if (input.keys[GLFW_KEY_D]) {
				cameraTransform->position.x += cameraSpeed * deltaTime * 10;
			}

			if (input.keys[GLFW_KEY_W]) {
				cameraTransform->position.z -= cameraSpeed * deltaTime * 10;
			}

			if (input.keys[GLFW_KEY_S]) {
				cameraTransform->position.z += cameraSpeed * deltaTime * 10;
			}

			glm::quat yawRotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
			cameraTransform->rotation = yawRotation * cameraTransform->rotation;

			glm::quat pitchRotation = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
			cameraTransform->rotation = pitchRotation * cameraTransform->rotation;

			transform.rotation = glm::rotate(transform.rotation, cube.rotationSpeed * deltaTime, cube.rotationAxis);
		});
	}

private:
	float cameraSpeed = 0.1f;
};
