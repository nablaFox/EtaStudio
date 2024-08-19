#pragma once

#include "demo_systems.hpp"
#include <glm/gtc/quaternion.hpp>

using namespace eta;

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

class PyramidMesh : public EtaMeshAsset {
public:
	using EtaMeshAsset::EtaMeshAsset;
	void setup() override {
		std::vector<Vertex> vertices(5);

		// Define the vertices for the pyramid
		vertices[0].position = {0.0f, 0.5f, 0.0f};	  // Top vertex
		vertices[1].position = {-0.5f, -0.5f, 0.5f};  // Base front-left
		vertices[2].position = {0.5f, -0.5f, 0.5f};	  // Base front-right
		vertices[3].position = {0.5f, -0.5f, -0.5f};  // Base back-right
		vertices[4].position = {-0.5f, -0.5f, -0.5f}; // Base back-left

		// Define UV coordinates (example values)
		for (int i = 0; i < 5; ++i) {
			vertices[i].uv_x = vertices[i].position.x + 0.5f;
			vertices[i].uv_y = vertices[i].position.y + 0.5f;
		}

		for (auto& vertex : vertices) {
			vertex.color = {1.0f, 1.0f, 1.0f, 1.0f};
		}

		std::vector<uint32_t> indices = {
			1, 2, 3, 3, 4, 1,
			// Faces (triangles)
			0, 1, 2, // Front face
			0, 2, 3, // Right face
			0, 3, 4, // Back face
			0, 4, 1	 // Left face
		};

		std::vector<MeshSurface> surfaces(5);
		surfaces[0] = {0, 6};  // Base
		surfaces[1] = {6, 3};  // Front face
		surfaces[2] = {9, 3};  // Right face
		surfaces[3] = {12, 3}; // Back face
		surfaces[4] = {15, 3}; // Left face

		setVertices(vertices);
		setIndices(indices);
		setSurfaces(surfaces);
	}
};

class GeometryScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		auto cube = addEntity();

		addRenderComponent(cube);
		addMeshComponent(cube, "CubeMesh");

		addComponent<RotationComponent>(cube,						  // entity
										0.5f,						  // rotationSpeed
										glm::vec3(1.0f, 0.0f, 0.0f)); // rotationAxis

		addComponent<InputComponent>(cube);

		auto pyramid = addEntity({
			.position = glm::vec3(-1.5f, 0.0f, 0.0f),
		});

		addRenderComponent(pyramid);
		addMeshComponent(pyramid, "PyramidMesh");

		addComponent<RotationComponent>(pyramid, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		addComponent<InputComponent>(pyramid);
		addComponent<MovementComponent>(pyramid, 1.0f);

		addDefaultCamera();
	};
};
