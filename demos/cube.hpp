#pragma once

#include "../include/eta_app.hpp"
#include <glm/gtc/quaternion.hpp>

using namespace eta;

struct CubeComponent {
	float rotationSpeed;
	glm::vec3 rotationAxis;
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
		auto entities = currentScene().getEntities<TransformComponent, CubeComponent>();

		entities.each([this, deltaTime](auto entity, TransformComponent& transform, CubeComponent& cube) {
			transform.rotation = glm::rotate(transform.rotation, cube.rotationSpeed * deltaTime, cube.rotationAxis);
		});
	}
};

class CubeScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		fmt::println("CubeScene setup");

		auto entity = addEntity();

		addRenderComponent(entity);
		addMeshComponent(entity, "CubeMesh");

		addComponent<CubeComponent>(entity,						  // entity
									0.5f,						  // rotationSpeed
									glm::vec3(1.0f, 0.0f, 0.0f)); // rotationAxis

		addDefaultCamera();
	};
};
