#pragma once

#include <entt.hpp>

#include "eta_pch.hpp"
#include "eta_asset.hpp"

namespace eta {

struct TransformComponent;
struct RenderComponent;
struct CameraComponent;

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

	entt::entity addEntity(TransformComponent transform);

	entt::entity addEntity();

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

	template <typename T>
	entt::entity getFirstWith() {
		auto view = m_entities.view<T>();
		return view.front();
	}

	template <typename... Components>
	auto getEntities() {
		auto entities = m_entities.view<Components...>();
		return entities;
	}

	void removeEntity(entt::entity entity) { m_entities.destroy(entity); }

	void addRenderComponent(entt::entity);
	void addRenderComponent(entt::entity entity, RenderComponent component);
	void addMeshComponent(entt::entity entity, str meshName);

	void addDefaultCamera();
	void addDefaultOrtographicCamera();

	entt::entity getActiveCamera();

	SceneSettings& settings() { return m_sceneSettings; }

private:
	entt::registry m_entities;
	entt::entity m_activeCamera = entt::null;

	SceneSettings m_sceneSettings;
};

};	// namespace eta
