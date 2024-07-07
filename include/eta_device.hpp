#pragma once

#include "eta_pch.hpp"
#include "eta_window.hpp"
#include "eta_utils.hpp"
#include "eta_descriptor.hpp"
#include "eta_swapchain.hpp"

namespace eta {

class EtaMeshAsset;
class EtaMaterial;
class EtaBindings;
class EtaPipeline;

class EtaDevice {
	struct FrameData {
		VkSemaphore _swapchainSemaphore, _renderSemaphore;
		VkFence _renderFence;

		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;

		etautil::DeletionQueue _deletionQueue;

		EtaDescriptorAllocator _globalDescriptorAllocator;
	};

public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	VkResult init(const EtaWindow& window);

	void destroy();

	VkDevice getDevice() const { return m_device; }

	AllocatedBuffer createUBO(size_t allocSize) {
		return createBuffer(allocSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
	}

	AllocatedBuffer createBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	// TODO: change these parameters to more raw data
	// instead of material and scene data, use descriptor sets
	// instead of mesh, use vertex buffer address, etc

	template <typename... Bindings>
	void drawMesh(const EtaMeshAsset& mesh, const glm::mat4& modelMatrix, const EtaMaterial& material, Bindings&&... bindings) {}

	VkResult drawFrame();
	VkResult endFrame();

	EtaDescriptorAllocator& getGlobalDescriptorAllocator() {
		return m_frames[m_currentFrame % MAX_FRAMES_IN_FLIGHT]._globalDescriptorAllocator;
	}

	template <typename... Bindings>
	std::shared_ptr<EtaPipeline> getPipeline(EtaMaterial& material, Bindings&&... bindings) {
		size_t bitmask = calculateBitmask(material, bindings...);
		if (m_pipelines.find(bitmask) != m_pipelines.end())
			return m_pipelines[bitmask];
		return registerPipeline(material, bindings...);
	}

	template <typename... Bindings>
	std::shared_ptr<EtaPipeline> registerPipeline(EtaMaterial& material, Bindings&&... bindings) {
		return nullptr;
	}

private:
	template <typename T>
	inline void hashCombine(std::size_t& seed, const T& value) {
		std::hash<T> hasher;
		seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template <typename Material, typename... Bindings>
	size_t calculateBitmask(Material& material, Bindings&&... bindings) {
		size_t materialHash = material.getUniqueIdentifier();

		(hashCombine(materialHash, bindings.getUniqueIdentifier()), ...);

		return materialHash;
	}

	VkDevice m_device = VK_NULL_HANDLE;
	EtaSwapchain m_swapchain;

	std::unordered_map<size_t, std::shared_ptr<EtaPipeline>> m_pipelines;

	std::vector<FrameData> m_frames;
	size_t m_currentFrame = 0;
};

}; // namespace eta
