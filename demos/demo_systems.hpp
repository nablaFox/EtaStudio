#pragma once

#include "../include/eta_system.hpp"
#include "../include/eta_default_components.hpp"

using namespace eta;

struct RotationComponent {
	float rotationSpeed;
	glm::vec3 rotationAxis;
};

struct MovementComponent {
	float speed;
};

class RotationSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update(float deltaTime) {
		auto& curr = currentScene();

		curr.getEntities<TransformComponent, RotationComponent>().each(
			[this, deltaTime, &curr](auto entity, TransformComponent& transform, RotationComponent& rot) {
				curr.setRotation(entity,
								 glm::rotate(transform.rotation, rot.rotationSpeed * deltaTime, rot.rotationAxis));
			});

		auto input = curr.getFirstWith<InputComponent>();

		if (input == entt::null)
			return;

		auto& inputComponent = curr.getComponent<InputComponent>(input);

		if (inputComponent.keys[GLFW_KEY_1]) {
			m_engine.switchScene("GeometryScene");
		}

		if (inputComponent.keys[GLFW_KEY_2]) {
			m_engine.switchScene("PendulumScene");
		}

		if (inputComponent.keys[GLFW_KEY_3]) {
			m_engine.switchScene("ViolinScene");
		}

		if (curr.getName() == "PendulumScene")
			return;
	}
};

class MovementSystem : public EtaSystem {
public:
	using EtaSystem::EtaSystem;

	void update(float deltaTime) {
		auto& curr = currentScene();

		auto toMove = curr.getFirstWith<MovementComponent>();
		auto input = curr.getFirstWith<InputComponent>();

		if (toMove == entt::null)
			return;

		if (input == entt::null)
			return;

		auto& inputComponent = curr.getComponent<InputComponent>(input);
		auto& movementComponent = curr.getComponent<MovementComponent>(toMove);

		glm::vec3 move = glm::vec3(0.0f);

		if (inputComponent.keys[GLFW_KEY_S]) {
			move.y -= deltaTime * movementComponent.speed;
		}

		if (inputComponent.keys[GLFW_KEY_W]) {
			move.y += deltaTime * movementComponent.speed;
		}

		if (inputComponent.keys[GLFW_KEY_A]) {
			move.x -= deltaTime * movementComponent.speed;
		}

		if (inputComponent.keys[GLFW_KEY_D]) {
			move.x += deltaTime * movementComponent.speed;
		}

		if (inputComponent.keys[GLFW_KEY_Q]) {
			move.z += deltaTime * movementComponent.speed;
		}

		if (inputComponent.keys[GLFW_KEY_E]) {
			move.z -= deltaTime * movementComponent.speed;
		}

		if (move != glm::vec3(0.0f))
			curr.updatePosition(toMove, move);
	}
};
