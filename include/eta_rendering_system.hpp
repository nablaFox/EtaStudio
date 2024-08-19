#pragma once

#include "eta_system.hpp"
#include "eta_device.hpp"

namespace eta {

struct CameraComponent;
struct TransformComponent;

class EtaGlobalSceneData : public EtaBindings {
public:
	EtaGlobalSceneData(EtaDevice& device) : m_device(device) {
		addBufferBinding(0);
		addMat4(0, "viewproj", glm::mat4(1.0f));
		addVec4(0, "ambientColor", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		ETA_CHECK(init(device, *device.getGlobalDescriptorAllocator()));
	}

	void setViewproj(const glm::mat4& viewproj) { setMat4(0, "viewproj", viewproj); }

	void update() { EtaBindings::updateBuffers(m_device); }

private:
	EtaDevice& m_device;
};

class EtaRenderingSystem : public EtaSystem {
public:
	EtaRenderingSystem(EtaEngine& engine, EtaWindow& window, EtaDevice& device)
		: EtaSystem(engine), m_window(window), m_device(device) {}

	void awake() override;

	void sleep() override { m_globalSceneData.destroy(m_device); }

	void update(float dt) override;

	void render3D();

	static glm::vec4 calculateViewRect(glm::vec4 rect, VkExtent2D viewport) {
		return {rect.x * viewport.width, rect.y * viewport.height, rect.z * viewport.width, rect.w * viewport.height};
	}

	static glm::mat4 calculateViewProjMatrix(CameraComponent& camera, TransformComponent& transform);

	static glm::mat4 calculateTransformMatrix(const TransformComponent& transform);

	static glm::mat4 calculateOrthoMatrix(CameraComponent& camera, TransformComponent& transform);

protected:
	EtaWindow& m_window;

private:
	EtaDevice& m_device;

	EtaGlobalSceneData m_globalSceneData{m_device};
};

};	// namespace eta
