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

		m_baseRenderingConfigs = {
			.inputTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_NONE,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.colorAttachmentFormat = m_drawImage.imageFormat,
			.depthFormat = VK_FORMAT_D32_SFLOAT,
			.vertexShader = VK_NULL_HANDLE,
			.fragmentShader = VK_NULL_HANDLE,
		};

		std::vector<std::string> defaultMaterials = {
			"default_metallic",
		};

		for (auto materialName : defaultMaterials) {
			auto material = m_engine.getMaterial(materialName);
			m_baseRenderingConfigs.fragmentShader = material->m_shader->m_fragShaderModule;
			m_baseRenderingConfigs.vertexShader = material->m_shader->m_vertShaderModule;

			m_device.registerGraphicsPipeline(m_baseRenderingConfigs, m_globalSceneData, *material);
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

			auto mesh = meshInfo.meshAsset;
			auto material = renderingInfo.material;

			// update material changes
			material->update();

			// update mesh changes
			mesh->update();

			// update global scene data
			m_globalSceneData.updateBuffers(m_device);

			// construct model matrix
			glm::mat4 modelMatrix = glm::mat4(1.0f);

			m_device.drawGeometry(mesh->m_gpuMeshData, modelMatrix, m_baseRenderingConfigs, m_globalSceneData, *material);
		}
	}

protected:
	EtaWindow& m_window;

private:
	EtaDevice& m_device;

	AllocatedImage m_drawImage;

	GraphicsPipelineConfigs m_baseRenderingConfigs;

	EtaGlobalSceneData m_globalSceneData{m_device};
};
}; // namespace eta
