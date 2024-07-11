#pragma once

#include "eta_pch.hpp"

namespace eta {

class EtaMeshAsset;
class EtaMaterial;

struct TransformComponent {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct CameraComponent {
	float fov;
	float aspect;
	float near;
	float far;
	bool enabled;
	glm::vec4 rect;
};

struct PointLightComponent {
	glm::vec3 color;
	float intensity;
};

struct DirectionalLightComponent {
	glm::vec3 color;
	glm::vec3 direction;
	float intensity;
};

struct MeshComponent {
	std::shared_ptr<EtaMeshAsset> meshAsset;
};

struct RenderComponent {
	std::shared_ptr<EtaMaterial> material;
};

}; // namespace eta
