#include "../include/eta_scene.hpp"
#include "../include/eta_asset_manager.hpp"

using namespace eta;

void EtaScene::addRenderComponent(entt::entity entity) {
	auto material = m_manager.getAsset<EtaMaterial>("default_metallic");
	m_entities.emplace<RenderComponent>(entity, material);
}

void EtaScene::addRenderComponent(entt::entity entity, str materialName) {
	auto material = m_manager.getAsset<EtaMaterial>(materialName);
	m_entities.emplace<RenderComponent>(entity, material);
}

void EtaScene::addMeshComponent(entt::entity entity, str meshName) {
	auto mesh = m_manager.getAsset<EtaMeshAsset>(meshName);
	m_entities.emplace<MeshComponent>(entity, mesh);
}
