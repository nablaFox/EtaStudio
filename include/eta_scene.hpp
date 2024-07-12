#pragma once

#include <entt.hpp>

#include "eta_pch.hpp"
#include "eta_asset.hpp"
#include "eta_default_components.hpp"

namespace eta {

class EtaScene : public EtaAsset {
public:
	using EtaAsset::EtaAsset;

	entt::entity addEntity() {
		auto entity = m_entities.create();
		m_entities.emplace<TransformComponent>(entity, glm::vec3(0), glm::quat(1, 0, 0, 0), glm::vec3(1.0f));
		return entity;
	}

	template <typename T, typename... Args>
	entt::entity addEntity(Args&&... args) {
		auto entity = addEntity();
		m_entities.emplace<T>(entity, std::forward<Args>(args)...);
		return entity;
	}

	template <typename T, typename... Args>
	void addComponent(entt::entity entity, Args&&... component) {
		m_entities.emplace<T>(entity, std::forward<Args>(component)...);
	}

	template <typename... Components>
	auto getEntities() {
		auto entities = m_entities.view<Components...>();
		return entities;
	}

	CameraComponent* getActiveCamera() {
		auto view = m_entities.view<CameraComponent>();
		for (auto entity : view) {
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.enabled)
				return &camera;
		}

		return nullptr;
	}

	TransformComponent* getActiveCameraTransform() {
		auto view = m_entities.view<CameraComponent, TransformComponent>();
		for (auto entity : view) {
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.enabled)
				return &view.get<TransformComponent>(entity);
		}

		return nullptr;
	}

	void removeEntity(entt::entity entity) { m_entities.destroy(entity); }

	void addRenderComponent(entt::entity);
	void addRenderComponent(entt::entity entity, RenderComponent& component);
	void addMeshComponent(entt::entity entity, str meshName);

	void addDefaultCamera();
	void addDefaultOrtographicCamera();

	struct GlobalSceneData {
		glm::vec4 ambientColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	} globalSceneData;

private:
	entt::registry m_entities;
};

}; // namespace eta
