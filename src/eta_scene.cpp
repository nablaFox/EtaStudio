#include "../include/eta_scene.hpp"
#include "../include/eta_asset_manager.hpp"

#include "../include/eta_default_components.hpp"

using namespace eta;

entt::entity EtaScene::addEntity(TransformComponent transform) {
	auto entity = m_entities.create();
	m_entities.emplace<TransformComponent>(entity, transform);
	return entity;
}

entt::entity EtaScene::addEntity() { return addEntity(TransformComponent()); }

void EtaScene::addDefaultCamera() {
	auto entity = m_entities.create();

	CameraComponent camera = {
		.fov = 50.0f,
		.aspect = 1.77f, // TODO: get from window
		.near = 0.1f,
		.far = 1000.0f,
		.enabled = true,
		.orthoSize = 0.0f,
	};

	TransformComponent transform = {
		.position = glm::vec3(0.0f, 0.0f, 5.0f),
		.rotation = glm::quat(1, 0, 0, 0),
		.scale = glm::vec3(1.0f),
	};

	m_entities.emplace<CameraComponent>(entity, camera);
	m_entities.emplace<TransformComponent>(entity, transform);
}

void EtaScene::addDefaultOrtographicCamera() {
	CameraComponent camera = {
		.fov = 0.0f,
		.aspect = 1.333f,
		.near = 0.1f,
		.far = 1000.0f,
		.enabled = true,
		.orthoSize = 1.0f,
	};

	addEntity<CameraComponent>(camera);
}

void EtaScene::addRenderComponent(entt::entity entity, RenderComponent component) {
	component.material =
		component.material ? component.material : m_manager.getAsset<EtaMaterial>("metallic_roughness");
	m_entities.emplace<RenderComponent>(entity, component);
}

void EtaScene::addRenderComponent(entt::entity entity) {
	RenderComponent component;
	addRenderComponent(entity, component);
}

void EtaScene::addMeshComponent(entt::entity entity, str meshName) {
	auto mesh = m_manager.getAsset<EtaMeshAsset>(meshName);
	m_entities.emplace<MeshComponent>(entity, mesh);
}

entt::entity EtaScene::getActiveCamera() {
	if (m_activeCamera != entt::null)
		return m_activeCamera;

	auto view = m_entities.view<CameraComponent>();

	for (auto entity : view) {
		auto camera = view.get<CameraComponent>(entity);
		if (camera.enabled) {
			m_activeCamera = entity;
			return entity;
		}
	}

	return entt::null;
}
