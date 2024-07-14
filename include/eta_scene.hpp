#pragma once

#include <entt.hpp>

#include "eta_pch.hpp"
#include "eta_asset.hpp"
#include "eta_default_components.hpp"

namespace eta {

class EtaScene : public EtaAsset {
public:
	using EtaAsset::EtaAsset;

	struct SceneSettings {
		glm::vec4 clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		glm::vec4 ambientColor = {0.1f, 0.1f, 0.1f, 1.0f};
	};

	void load() override {
		fmt::println("Info: Scene {} loaded", getName());
		// TODO: here set the initial viewport
	}

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

	template <typename T>
	T& getComponent(entt::entity entity) {
		return m_entities.get<T>(entity);
	}

	template <typename... Components>
	auto getEntities() {
		auto entities = m_entities.view<Components...>();
		return entities;
	}

	void removeEntity(entt::entity entity) { m_entities.destroy(entity); }

	void addRenderComponent(entt::entity);
	void addRenderComponent(entt::entity entity, RenderComponent& component);
	void addMeshComponent(entt::entity entity, str meshName);

	void addDefaultCamera();
	void addDefaultOrtographicCamera();

	CameraComponent* getActiveCamera() {
		if (m_activeCamera != entt::null)
			return &m_entities.get<CameraComponent>(m_activeCamera);

		auto view = m_entities.view<CameraComponent>();
		for (auto entity : view) {
			auto& camera = view.get<CameraComponent>(entity);
			if (camera.enabled) {
				m_activeCamera = entity;
				return &camera;
			}
		}

		return nullptr;
	}

	TransformComponent* getActiveCameraTransform() {
		if (m_activeCamera != entt::null)
			return &m_entities.get<TransformComponent>(m_activeCamera);

		auto view = m_entities.view<TransformComponent>();
		for (auto entity : view) {
			if (entity == m_activeCamera) {
				return &view.get<TransformComponent>(entity);
			}
		}

		return nullptr;
	}

	SceneSettings& settings() { return m_sceneSettings; }

private:
	entt::registry m_entities;
	entt::entity m_activeCamera = entt::null;

	SceneSettings m_sceneSettings;
};

}; // namespace eta
