#pragma once

#include "demo_systems.hpp"

using namespace eta;

class ViolinModel : public EtaModelAsset {
public:
	using EtaModelAsset::EtaModelAsset;
	void setup() { setModelPath("assets/violin.glb"); }
};

class ViolinScene : public EtaScene {
public:
	using EtaScene::EtaScene;

	void setup() override {
		auto violin = addEntity({
			.position = glm::vec3(0.0f, -2.5f, 3.0f),
			.rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
			.scale = glm::vec3(0.1f),
		});

		addRenderComponent(violin, {.material = m_manager.getAsset<EtaMaterial>("violin_material_Body")});
		addMeshComponent(violin, "violin_mesh_Violin_Body_0");

		addComponent<InputComponent>(violin);
		addComponent<MovementComponent>(violin, 1.0f);

		addDefaultCamera();
	}
};
