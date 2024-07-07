#pragma once

#include <entt.hpp>

#include "ets_pch.hpp"
#include "ets_asset.hpp"
#include "ets_default_components.hpp"

namespace ets {

class EtaSceneAsset : public EtaAsset {
public:
	entt::entity addEntity() {
		auto entity = m_entities.create();
		m_entities.emplace<TransformComponent>(entity, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
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

	void removeEntity(entt::entity entity) { m_entities.destroy(entity); }

private:
	entt::registry m_entities;
};

}; // namespace ets
