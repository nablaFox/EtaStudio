#pragma once

#include "ets_pch.hpp"

namespace ets {

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

}; // namespace ets
