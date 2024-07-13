#pragma once

#include "rotation_system.hpp"
#include "../include/eta_app.hpp"

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
		auto violin = addEntity();

		RenderComponent renderComponent{
			.material = m_manager.getAsset<EtaMaterial>("violin_material_Body"),
		};
		addRenderComponent(violin, renderComponent);
		addMeshComponent(violin, "violin_mesh_Violin_Body_0");

		addComponent<RotationComponent>(violin, 0.08f, glm::vec3(0.0f, 1.0f, 0.0f));

		getComponent<TransformComponent>(violin).scale = glm::vec3(0.1f);

		// flip y axis
		getComponent<TransformComponent>(violin).rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		addComponent<InputComponent>(violin);

		addDefaultCamera();
	}
};
