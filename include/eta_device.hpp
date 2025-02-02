#pragma once

#include "eta_bindings.hpp"
#include "eta_pch.hpp"
#include "eta_graphics_pipeline.hpp"
#include "eta_utils.hpp"
#include "eta_descriptor.hpp"
#include "eta_swapchain.hpp"

namespace eta {

class EtaWindow;

class EtaDevice {
	friend class EtaSwapchain;

public:
	struct FrameData {
		VkSemaphore _swapchainSemaphore, _renderSemaphore;
		VkFence _renderFence;

		VkCommandPool _commandPool;
		VkCommandBuffer _commandBuffer;

		etautil::DeletionQueue _deletionQueue;
	};

	struct ImmediateData {
		VkCommandBuffer _commandBuffer;
		VkCommandPool _commandPool;
		VkFence _fence;
	};

	struct DrawPushConstants {
		glm::mat4 _transform;
		VkDeviceAddress _vertexBufferAddress;
	};

	struct DrawCallOpts {
		GPUMeshData meshData;
		glm::mat4 transform;
	};

	struct GraphicsPipelineConfigs {
		VkPrimitiveTopology inputTopology;
		VkPolygonMode polygonMode;
		VkCullModeFlags cullMode;
		VkFrontFace frontFace;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkShaderModule vertexShader;
		VkShaderModule fragmentShader;
	};

public:
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
	static constexpr bool ENABLE_VALIDATION_LAYERS = true;

	VkResult init(EtaWindow& window);

	void destroy();

	VkDevice getDevice() const { return m_device; }

	std::shared_ptr<EtaDescriptorAllocator> getGlobalDescriptorAllocator() { return m_globalDescriptorAllocator; }

	VkResult startFrame(glm::vec4 viewport, glm::vec4 clearColor);
	VkResult endFrame();

	VkCommandBuffer& currentCmd() { return getCurrentFrame()._commandBuffer; }

	FrameData& getCurrentFrame() { return m_frames[m_currentFrame % MAX_FRAMES_IN_FLIGHT]; }

	void waitIdle() { vkDeviceWaitIdle(m_device); }

	template <typename... Bindings>
	void bindResources(GPUMeshData& meshData,
					   glm::mat4 modelMatrix,
					   GraphicsPipelineConfigs& pipelineConfigs,
					   Bindings&&... bindings) {
		auto cmd = currentCmd();

		auto pipeline =
			std::static_pointer_cast<EtaGraphicsPipeline>(getGraphicsPipeline(pipelineConfigs, bindings...));

		auto bindDescriptorSets = [&cmd, &pipeline](auto& binding, int index) {
			pipeline->bindDescriptorSet(cmd, binding.getDescriptorSet(), index);
		};

		int i = 0;
		((bindDescriptorSets(bindings, i++)), ...);

		DrawPushConstants pushConstants = {
			._transform = modelMatrix,
			._vertexBufferAddress = meshData.vertexBufferAddress,
		};

		pipeline->template pushConstants<DrawPushConstants>(cmd, pushConstants);
		pipeline->bind(cmd);

		vkCmdBindIndexBuffer(cmd, meshData.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void drawIndexed(size_t indexCount,
					 size_t instanceCount,
					 size_t firstIndex,
					 size_t vertexOffset,
					 size_t firstInstance) {
		auto cmd = currentCmd();
		vkCmdDrawIndexed(cmd, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	template <typename... Bindings>
	std::shared_ptr<EtaPipeline> getGraphicsPipeline(GraphicsPipelineConfigs& configs, Bindings&&... bindings) {
		size_t bitmask = calculateHash(configs, bindings...);  // TODO: avoid recalculating hash every time
		if (m_pipelines.find(bitmask) != m_pipelines.end())
			return m_pipelines[bitmask];

		fmt::print("Debug: Graphics Pipeline not found, creating new one. Hash {} \n", bitmask);
		return registerGraphicsPipeline(configs, bindings...);
	}

	template <typename... Bindings>
	std::shared_ptr<EtaPipeline> registerGraphicsPipeline(GraphicsPipelineConfigs configs, Bindings&&... bindings) {
		auto newPipeline = std::make_shared<EtaGraphicsPipeline>();

		newPipeline->setInputTopology(configs.inputTopology);
		newPipeline->setPolygonMode(configs.polygonMode);
		newPipeline->setShaders(configs.vertexShader, configs.fragmentShader);
		newPipeline->setCullMode(configs.cullMode, configs.frontFace);
		newPipeline->disableBlending();	 // temp

		// default
		newPipeline->setMultisampling(m_msaaSamples);
		newPipeline->setDepthTest();
		newPipeline->setColorAttachmentFormat(m_drawImage.imageFormat);
		newPipeline->setDepthFormat(m_depthImage.imageFormat);
		newPipeline->setPushConstantRange(m_graphicsPushConstantRange);

		(
			[&](auto&& binding) {
				newPipeline->addSlotLayout(std::forward<decltype(binding)>(binding).getDescriptorSetLayout());
			}(bindings),
			...);

		newPipeline->build(m_device);

		fmt::print("Debug: Graphics Pipeline built\n");

		return m_pipelines[calculateHash(configs, bindings...)] = newPipeline;
	}

public:
	VkResult createBuffer(size_t allocSize,
						  VkBufferUsageFlags usage,
						  VmaMemoryUsage memoryUsage,
						  AllocatedBuffer& buffer);
	VkResult createUBO(size_t allocSize, AllocatedBuffer& uboBuffer);
	VkResult fillBuffer(AllocatedBuffer& buffer, void* data, size_t size, size_t offset);
	VkResult createStagingBuffer(size_t allocSize, AllocatedBuffer& stagingBuffer, void*& data);
	VkResult destroyBuffer(AllocatedBuffer& buffer);

	VkResult uploadMesh(std::span<Vertex> vertices, std::span<Index> indices, GPUMeshData& meshData);
	VkResult updateMesh(std::span<Vertex> vertices, std::span<Index> indices, GPUMeshData& meshData);
	VkResult destroyMesh(GPUMeshData& meshData);

	VkResult createImage(VkExtent3D size,
						 VkFormat format,
						 VkImageUsageFlags usage,
						 AllocatedImage& handle,
						 VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT,
						 bool mipmapped = false);
	VkResult createSampler(VkSampler* sampler, VkSamplerCreateInfo* info);
	VkResult fillImage(AllocatedImage& image, void* data);
	VkResult createFilledImage(AllocatedImage& image,
							   void* data,
							   VkExtent3D size,
							   VkFormat format,
							   VkImageUsageFlags usage);
	VkResult destroySampler(VkSampler* sampler);
	VkResult destroyImage(AllocatedImage& handle);
	VkResult destroySampler(VkSampler& sampler);

	VkResult createDrawImage(VkExtent2D size,
							 AllocatedImage& handle,
							 VkSampleCountFlagBits numSamples = VK_SAMPLE_COUNT_1_BIT);
	VkResult createDepthImage(VkExtent2D size, AllocatedImage& handle);

	VkResult createCommandPool(VkCommandPool* pool, VkCommandPoolCreateFlags flags = 0);
	VkResult allocateCommandBuffer(VkCommandBuffer* buffer, VkCommandPool pool);

	VkResult createSemaphore(VkSemaphore* semaphore, VkSemaphoreCreateFlags flags = 0);
	VkResult createFence(VkFence* fence, VkFenceCreateFlags flags = 0);

	VkResult createShaderModule(std::vector<uint32_t>& code, VkShaderModule& shader);
	VkResult destroyShaderModule(VkShaderModule& shader);

	VkResult submitCommand(int submitCount, VkSubmitInfo2* submitInfo, VkFence fence = VK_NULL_HANDLE);
	VkResult immediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function);

private:
	template <typename... Bindings>
	size_t calculateHash(GraphicsPipelineConfigs& configs, Bindings&&... bindings) {
		size_t hash = 0;

		etautil::hashCombine(hash, configs.inputTopology);
		etautil::hashCombine(hash, configs.polygonMode);
		etautil::hashCombine(hash, configs.cullMode);
		etautil::hashCombine(hash, configs.frontFace);
		etautil::hashCombine(hash, configs.vertexShader);
		etautil::hashCombine(hash, configs.fragmentShader);

		(etautil::hashCombine(hash, bindings.getHash()), ...);

		return hash;
	}

	VkInstance m_vkInstance;
	VkDebugUtilsMessengerEXT m_debugMessenger;
	VkSurfaceKHR m_surface;
	VkPhysicalDevice m_chosenGPU;
	VkDevice m_device;
	ImmediateData m_immData;
	EtaSwapchain m_swapchain{*this};
	VkExtent2D m_drawExtent;
	VkPushConstantRange m_graphicsPushConstantRange;

	VmaAllocator m_allocator;

	VkQueue m_graphicsQueue;
	uint32_t m_graphicsQueueFamily;

	etautil::DeletionQueue m_deletionQueue;

	std::unordered_map<size_t, std::shared_ptr<EtaPipeline>> m_pipelines;

	std::shared_ptr<EtaDescriptorAllocator> m_globalDescriptorAllocator;

	FrameData m_frames[MAX_FRAMES_IN_FLIGHT];
	size_t m_currentFrame = 0;

	VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	AllocatedImage m_drawImage;
	AllocatedImage m_resolveImage;
	AllocatedImage m_depthImage;
};

};	// namespace eta
