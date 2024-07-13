#pragma once

#include "../include/eta_app.hpp"

using namespace eta;

struct RotationComponent {
	float rotationSpeed;
	glm::vec3 rotationAxis;
};

class RotationSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update(float deltaTime) {
		auto& curr = currentScene();
		auto cameraTransform = currentScene().getActiveCameraTransform();

		curr.getEntities<TransformComponent, RotationComponent>().each(
			[this, deltaTime](auto entity, TransformComponent& transform, RotationComponent& rot) {
				transform.rotation = glm::rotate(transform.rotation, rot.rotationSpeed * deltaTime, rot.rotationAxis);
			});

		auto input = curr.getComponent<InputComponent>(curr.getEntities<InputComponent>().front());

		if (input.keys[GLFW_KEY_1]) {
			m_engine.switchScene("CubeScene");
		}

		if (input.keys[GLFW_KEY_2]) {
			m_engine.switchScene("PendulumScene");
		}

		if (input.keys[GLFW_KEY_3]) {
			m_engine.switchScene("ViolinScene");
		}

		if (curr.getName() == "PendulumScene")
			return;

		float pitch = 0.0f;
		float yaw = 0.0f;
		float cameraSensitivity = 0.02f;
		float movSpeed = 0.1f;

		if (input.keys[GLFW_KEY_UP]) {
			pitch -= cameraSensitivity;
		}

		if (input.keys[GLFW_KEY_DOWN]) {
			pitch += cameraSensitivity;
		}

		if (input.keys[GLFW_KEY_LEFT]) {
			yaw += cameraSensitivity;
		}

		if (input.keys[GLFW_KEY_RIGHT]) {
			yaw -= cameraSensitivity;
		}

		// update camera rotation, both pitch and yaw
		cameraTransform->rotation = glm::rotate(cameraTransform->rotation, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		cameraTransform->rotation = glm::rotate(cameraTransform->rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

		// we have to calculate the forward and right vector ofw the camera
		glm::vec3 forward = glm::normalize(cameraTransform->position - cameraTransform->position -
										   cameraTransform->rotation * glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

		if (input.keys[GLFW_KEY_W])
			cameraTransform->position += forward * movSpeed;

		if (input.keys[GLFW_KEY_S])
			cameraTransform->position -= forward * movSpeed;

		if (input.keys[GLFW_KEY_A])
			cameraTransform->position -= right * movSpeed;

		if (input.keys[GLFW_KEY_D])
			cameraTransform->position += right * movSpeed;
	}
};
