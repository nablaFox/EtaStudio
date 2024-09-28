#include "../include/eta_rendering_system.hpp"

#include "../include/eta_default_components.hpp"
#include "../include/eta_mesh.hpp"
#include "../include/eta_scene.hpp"

#include "../include/eta_default_components.hpp"

using namespace eta;

void EtaRenderingSystem::awake() {
	EtaDevice::GraphicsPipelineConfigs baseRenderingConfigs = {
		.inputTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
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

void EtaRenderingSystem::update(float dt) {
	EtaScene::SceneSettings sceneSettings = currentScene().settings();

	currentScene().updateTransforms();

	currentScene().getEntities<CameraComponent, TransformComponent>().each([&](auto entity, CameraComponent& camera,
																			   TransformComponent& transform) {
		if (!camera.enabled)
			return;

		ETA_CHECK(m_device.startFrame(calculateViewRect(camera.rect, m_window.getExtent()), sceneSettings.clearColor));

		// TEMP
		camera.aspect = m_window.width() / static_cast<float>(m_window.height());

		glm::mat4 viewproj = camera.orthoSize == 0.0f ? calculateViewProjMatrix(camera, transform)
													  : calculateOrthoMatrix(camera, transform);

		m_globalSceneData.setViewproj(viewproj);

		m_globalSceneData.update();

		render3D();

		ETA_CHECK(m_device.endFrame());
	});
}

void EtaRenderingSystem::render3D() {
	auto entities = currentScene().getEntities<RenderComponent, MeshComponent, TransformComponent>();

	for (auto entity : entities) {
		RenderComponent renderingInfo = entities.get<RenderComponent>(entity);
		MeshComponent meshInfo = entities.get<MeshComponent>(entity);
		TransformComponent transform = entities.get<TransformComponent>(entity);

		auto mesh = meshInfo.meshAsset;
		auto material = renderingInfo.material;
		auto transformMatrix = transform.m_cachedTransform;

		if (!mesh || !material)
			continue;

		material->update();

		mesh->update();

		EtaDevice::GraphicsPipelineConfigs renderingConfigs = {
			.inputTopology = renderingInfo.topology,
			.polygonMode = renderingInfo.polygonMode,
			.cullMode = renderingInfo.cullMode,
			.frontFace = renderingInfo.frontFace,
			.vertexShader = material->m_shader->m_vertShaderModule,
			.fragmentShader = material->m_shader->m_fragShaderModule,
		};

		m_device.bindResources(mesh->m_gpuMeshData, transformMatrix, renderingConfigs, m_globalSceneData, *material);

		if (mesh->m_meshSurfaces.empty())
			return m_device.drawIndexed(mesh->m_indices.size(), 1, 0, 0, 0);

		for (auto& surface : mesh->m_meshSurfaces)
			m_device.drawIndexed(surface.indexCount, 1, surface.firstIndex, 0, 0);
	}
}

glm::mat4 EtaRenderingSystem::calculateViewProjMatrix(CameraComponent& camera, TransformComponent& transform) {
	glm::mat4 proj = glm::mat4(0.0f);
	const float tanHalfFovy = tan(glm::radians(camera.fov) / 2.f);

	proj[0][0] = 1.f / (camera.aspect * tanHalfFovy);
	proj[1][1] = -1.f / (tanHalfFovy);
	proj[2][2] = camera.far / (camera.far - camera.near);
	proj[2][3] = 1.f;
	proj[3][2] = -(camera.far * camera.near) / (camera.far - camera.near);

	return proj * calculateTransformMatrix(transform);
}

glm::mat4 EtaRenderingSystem::calculateTransformMatrix(const TransformComponent& transform) {
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.position);
	glm::mat4 rotationMatrix = glm::mat4_cast(transform.rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);
	return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::mat4 EtaRenderingSystem::calculateOrthoMatrix(CameraComponent& camera, TransformComponent& transform) {
	float orthoSize = camera.orthoSize;
	float aspectRatio = camera.aspect;

	float left = -orthoSize * aspectRatio;
	float right = orthoSize * aspectRatio;
	float bottom = -orthoSize;
	float top = orthoSize;

	return glm::ortho(left, right, bottom, top) * calculateTransformMatrix(transform);
}
