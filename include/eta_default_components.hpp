#pragma once

#include "eta_pch.hpp"

namespace eta {

class EtaMeshAsset;
class EtaMaterial;

struct TransformComponent {
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

struct CameraComponent {
	float fov;
	float aspect;
	float near;
	float far;
	bool enabled;
	float orthoSize;
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
	VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
	VkCullModeFlags cullMode = VK_CULL_MODE_NONE;
	VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
};

struct InputComponent {
	bool keys[GLFW_KEY_LAST] = {false};
	bool prevKeys[GLFW_KEY_LAST] = {false};
	bool singleKeyPresses[GLFW_KEY_LAST] = {false};
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST] = {false};
	double mouseX = 0.0;
	double mouseY = 0.0;
};

}; // namespace eta
