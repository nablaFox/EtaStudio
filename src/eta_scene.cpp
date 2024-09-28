#include "../include/eta_scene.hpp"
#include "../include/eta_asset_manager.hpp"

#include "../include/eta_default_components.hpp"

#include "../include/eta_rendering_system.hpp"

using namespace eta;

entt::entity EtaScene::addEntity(TransformComponent transform) {
	auto entity = m_entities.create();
	m_entities.emplace<TransformComponent>(entity, transform);
	m_entities.emplace<HierarchyComponent>(entity);
	return entity;
}

entt::entity EtaScene::addEntity() {
	return addEntity(TransformComponent());
}

void EtaScene::setParent(entt::entity child, entt::entity parent) {
	auto& hierarchy = getComponent<HierarchyComponent>(child);
	hierarchy.parent = parent;

	auto& parentHierarchy = getComponent<HierarchyComponent>(parent);
	parentHierarchy.children.push_back(child);

	markDirty(child);
}

void EtaScene::updateTransforms() {
	std::function<void(entt::entity)> updateEntityTransform = [&](entt::entity entity) {
		auto& transform = getComponent<TransformComponent>(entity);

		if (!transform.m_dirty)
			return;

		transform.m_dirty = false;

		transform.m_cachedTransform = EtaRenderingSystem::calculateTransformMatrix(transform);
		auto& hierarchy = getComponent<HierarchyComponent>(entity);

		entt::entity parent = hierarchy.parent;

		if (parent != entt::null) {
			updateEntityTransform(parent);

			auto& parentTransform = getComponent<TransformComponent>(parent).m_cachedTransform;
			transform.m_cachedTransform = parentTransform * transform.m_cachedTransform;
		}
	};

	auto rootView = m_entities.view<TransformComponent, HierarchyComponent>();

	for (auto entity : rootView) {
		updateEntityTransform(entity);
	}
}

void EtaScene::markDirty(entt::entity entity) {
	getComponent<TransformComponent>(entity).m_dirty = true;

	auto& hierarchy = getComponent<HierarchyComponent>(entity);
	for (auto child : hierarchy.children)
		markDirty(child);
}

void EtaScene::setPosition(entt::entity entity, glm::vec3 position) {
	getComponent<TransformComponent>(entity).position = position;
	markDirty(entity);
}

void EtaScene::setRotation(entt::entity entity, glm::quat rotation) {
	getComponent<TransformComponent>(entity).rotation = rotation;
	markDirty(entity);
}

void EtaScene::setScale(entt::entity entity, glm::vec3 scale) {
	getComponent<TransformComponent>(entity).scale = scale;
	markDirty(entity);
}

void EtaScene::updatePosition(entt::entity entity, glm::vec3 position) {
	getComponent<TransformComponent>(entity).position += position;
	markDirty(entity);
}

entt::entity EtaScene::addDefaultCamera(TransformComponent transform) {
	auto entity = m_entities.create();

	CameraComponent camera = {
		.fov = 50.0f,
		.aspect = 1.77f,  // TODO: get from window
		.near = 0.1f,
		.far = 1000.0f,
		.enabled = true,
		.orthoSize = 0.0f,
	};

	m_entities.emplace<CameraComponent>(entity, camera);
	m_entities.emplace<TransformComponent>(entity, transform);

	return entity;
}

entt::entity EtaScene::addDefaultCamera() {
	return addDefaultCamera({
		.position = glm::vec3(0.0f, 0.0f, 5.0f),
		.rotation = glm::quat(1, 0, 0, 0),
		.scale = glm::vec3(1.0f),
	});
}

entt::entity EtaScene::addDefaultOrtographicCamera() {
	CameraComponent camera = {
		.fov = 0.0f,
		.aspect = 1.333f,
		.near = 0.1f,
		.far = 1000.0f,
		.enabled = true,
		.orthoSize = 1.0f,
	};

	return addEntity<CameraComponent>(camera);
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
