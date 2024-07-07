#pragma once

#include "eta_utils.hpp"
#include "eta_system.hpp"

namespace eta {

class EtaGlobalSceneData : public EtaBindings {
public:
	EtaGlobalSceneData(EtaDevice& device) {
		EtaBufferBinding binding;

		binding.setProperty("view", glm::mat4(1.0f));
		binding.setProperty("projection", glm::mat4(1.0f));

		addBufferBinding(0, binding);

		init(device, device.getGlobalDescriptorAllocator());
	}
};

class EtaRenderingSystem : public EtaSystem {
public:
	EtaRenderingSystem(EtaEngine& engine, EtaWindow& window, EtaDevice& device)
		: EtaSystem(engine), m_window(window), m_device(device) {}

	void awake() override {
		EtaBufferBinding binding;

		binding.setProperty("view", glm::mat4(1.0f));
		binding.setProperty("projection", glm::mat4(1.0f));

		m_globalSceneData.addBufferBinding(0, binding);

		// setup default pipelines
		auto metallicMaterial = m_engine.getMaterial("default_metallic");
		m_device.registerPipeline(*metallicMaterial, m_globalSceneData);
	}

	void update() override {
		ETA_CHECK(m_device.drawFrame());

		auto cameras = currentScene().getEntities<CameraComponent>();

		for (auto entity : cameras) {
			CameraComponent camera = cameras.get<CameraComponent>(entity);

			if (!camera.enabled)
				continue;

			render3D();
		}

		ETA_CHECK(m_device.endFrame());
	}

	void render3D() {
		auto entities = currentScene().getEntities<RenderComponent, MeshComponent>();

		for (auto entity : entities) {
			RenderComponent renderingInfo = entities.get<RenderComponent>(entity);
			MeshComponent meshInfo = entities.get<MeshComponent>(entity);
			auto material = m_engine.getMaterial(renderingInfo.materialName);

			// update material changes
			material->updateBuffers(m_device);

			// update global scene data
			m_globalSceneData.updateBuffers(m_device);

			// draw mesh
			auto mesh = m_engine.getMesh(meshInfo.meshName);

			// construct model matrix
			glm::mat4 modelMatrix = glm::mat4(1.0f);

			m_device.drawMesh(*mesh, modelMatrix, *material, m_globalSceneData);
		}
	}

protected:
	EtaWindow& m_window;

private:
	EtaDevice& m_device;

	EtaGlobalSceneData m_globalSceneData{m_device};
};
}; // namespace eta
