#pragma once

#include "eta_utils.hpp"
#include "eta_system.hpp"
#include "eta_device.hpp"

namespace eta {

class EtaGlobalSceneData : public EtaBindings {
public:
	EtaGlobalSceneData(EtaDevice& device) : m_device(device) {
		addBufferBinding(0);
		addMat4(0, "viewproj", glm::mat4(1.0f));
		addVec4(0, "ambientColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		ETA_CHECK(init(device, *device.getGlobalDescriptorAllocator()));
	}

	void update() { EtaBindings::updateBuffers(m_device); }

private:
	EtaDevice& m_device;
};

class EtaRenderingSystem : public EtaSystem {
public:
	EtaRenderingSystem(EtaEngine& engine, EtaWindow& window, EtaDevice& device)
		: EtaSystem(engine), m_window(window), m_device(device) {}

	void awake() override {
		GraphicsPipelineConfigs baseRenderingConfigs = {
			.inputTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_NONE,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.enableDepthTest = true,
		};

		std::vector<std::string> defaultMaterials = {
			"metallic_roughness",
		};

		for (auto materialName : defaultMaterials) {
			auto material = m_engine.getMaterial(materialName);
			baseRenderingConfigs.fragmentShader = material->m_shader->m_fragShaderModule;
			baseRenderingConfigs.vertexShader = material->m_shader->m_vertShaderModule;

			m_device.registerGraphicsPipeline(baseRenderingConfigs, m_globalSceneData, *material);
		}
	}

	void sleep() override { m_globalSceneData.destroy(m_device); }

	void update(float dt) override {
		EtaScene::SceneSettings sceneSettings = currentScene().settings();

		currentScene().getEntities<CameraComponent, TransformComponent>().each(
			[&](auto entity, CameraComponent& camera, TransformComponent& transform) {
				if (!camera.enabled)
					return;

				ETA_CHECK(m_device.startFrame(calculateViewRect(camera.rect, m_window.getExtent()), sceneSettings.clearColor));

				if (camera.orthoSize == 0.0f)
					m_globalSceneData.setMat4(0, "viewproj", calculateViewProjMatrix(camera, transform));
				else
					m_globalSceneData.setMat4(0, "viewproj", calculateOrthoMatrix(camera, transform));

				m_globalSceneData.update();

				render3D();

				ETA_CHECK(m_device.endFrame());
			});
	}

	void render3D() {
		auto entities = currentScene().getEntities<RenderComponent, MeshComponent, TransformComponent>();

		for (auto entity : entities) {
			RenderComponent renderingInfo = entities.get<RenderComponent>(entity);
			MeshComponent meshInfo = entities.get<MeshComponent>(entity);
			TransformComponent transform = entities.get<TransformComponent>(entity);

			auto mesh = meshInfo.meshAsset;
			auto material = renderingInfo.material;
			auto transformMatrix = calculateTransformMatrix(transform);

			if (!mesh || !material)
				continue;

			material->update();

			mesh->update();

			GraphicsPipelineConfigs renderingConfigs = {
				.inputTopology = renderingInfo.topology,
				.polygonMode = renderingInfo.polygonMode,
				.cullMode = renderingInfo.cullMode,
				.frontFace = renderingInfo.frontFace,
				.vertexShader = material->m_shader->m_vertShaderModule,
				.fragmentShader = material->m_shader->m_fragShaderModule,
				.enableDepthTest = true // TEMP: should be configurable
			};

			m_device.bindResources(mesh->m_gpuMeshData, transformMatrix, renderingConfigs, m_globalSceneData, *material);

			if (mesh->m_meshSurfaces.empty())
				return m_device.drawIndexed(mesh->m_indices.size(), 1, 0, 0, 0);

			for (auto& surface : mesh->m_meshSurfaces)
				m_device.drawIndexed(surface.indexCount, 1, surface.firstIndex, 0, 0);
		}
	}

	static glm::vec4 calculateViewRect(glm::vec4 rect, VkExtent2D viewport) {
		return {rect.x * viewport.width, rect.y * viewport.height, rect.z * viewport.width, rect.w * viewport.height};
	}

	static glm::mat4 calculateViewProjMatrix(CameraComponent& camera, TransformComponent& transform) {
		glm::mat4 projMatrix = glm::perspective(glm::radians(camera.fov), camera.aspect, camera.near, camera.far);
		glm::mat4 transformMatrix = calculateTransformMatrix(transform);
		return projMatrix * glm::inverse(transformMatrix);
	}

	static glm::mat4 calculateTransformMatrix(const TransformComponent& transform) {
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.position);
		glm::mat4 rotationMatrix = glm::mat4_cast(transform.rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);
		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	static glm::mat4 calculateOrthoMatrix(CameraComponent& camera, TransformComponent& transform) {
		float orthoSize = camera.orthoSize;
		float aspectRatio = camera.aspect;

		float left = -orthoSize * aspectRatio;
		float right = orthoSize * aspectRatio;
		float bottom = -orthoSize;
		float top = orthoSize;

		return glm::ortho(left, right, bottom, top) * calculateTransformMatrix(transform);
	}

protected:
	EtaWindow& m_window;

private:
	EtaDevice& m_device;

	EtaGlobalSceneData m_globalSceneData{m_device};
};
}; // namespace eta
