#pragma once

#include "eta_utils.hpp"
#include "eta_system.hpp"
#include "eta_device.hpp"

namespace eta {

class EtaGlobalSceneData : public EtaBindings {
public:
	EtaGlobalSceneData(EtaDevice& device) {
		addBufferBinding(0);
		setBufferProperty(0, "view", glm::mat4(1.0f));
		setBufferProperty(0, "projection", glm::mat4(1.0f));

		ETA_CHECK(init(device, *device.getGlobalDescriptorAllocator()));
	}
};

class EtaRenderingSystem : public EtaSystem {
public:
	EtaRenderingSystem(EtaEngine& engine, EtaWindow& window, EtaDevice& device)
		: EtaSystem(engine), m_window(window), m_device(device) {}

	void awake() override {
		ETA_CHECK(m_device.createDrawImage(m_window.getExtent(), m_drawImage));

		m_basePipelineConfigs = {
			.colorAttachmentFormat = m_drawImage.imageFormat,
		};

		std::vector<std::string> defaultMaterials = {
			"default_metallic",
		};

		RenderingConfigs renderingConfigs{.pipelineConfigs = m_basePipelineConfigs};

		for (auto materialName : defaultMaterials) {
			auto material = m_engine.getMaterial(materialName);
			renderingConfigs.fragmentShader = material->m_shader->m_fragShaderModule;
			renderingConfigs.vertexShader = material->m_shader->m_vertShaderModule;

			m_device.registerGraphicsPipeline(renderingConfigs, *material, m_globalSceneData);
		}
	}

	void sleep() override {
		fmt::print("EtaRenderingSystem::sleep()\n");
		m_globalSceneData.destroy(m_device);
	}

	void update() override {
		ETA_CHECK(m_device.startFrame(m_drawImage));

		auto cameras = currentScene().getEntities<CameraComponent>();

		for (auto entity : cameras) {
			CameraComponent camera = cameras.get<CameraComponent>(entity);

			if (!camera.enabled)
				continue;

			render3D();
		}

		ETA_CHECK(m_device.endFrame(m_drawImage));
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

			EtaDevice::DrawCallOpts opts = {
				.meshData = mesh->m_gpuMeshData,
				.vertexBufferAddress = mesh->m_gpuMeshData.vertexBufferAddress,
				.vertexShader = material->m_shader->m_vertShaderModule,
				.fragmentShader = material->m_shader->m_fragShaderModule,
				.model = modelMatrix,
			};

			m_device.drawGeometry(opts, m_basePipelineConfigs, *material, m_globalSceneData);
		}
	}

protected:
	EtaWindow& m_window;

private:
	EtaDevice& m_device;

	AllocatedImage m_drawImage;

	GraphicsPipelineConfigs m_basePipelineConfigs;

	EtaGlobalSceneData m_globalSceneData{m_device};
};
}; // namespace eta
