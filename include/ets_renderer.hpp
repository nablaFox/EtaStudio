#pragma once

#include "ets_pch.hpp"
#include "ets_utils.hpp"
#include "ets_descriptor_allocator.hpp"

namespace ets {

class EtaWindow;

struct FrameData {
	VkSemaphore _swapchainSemaphore, _renderSemaphore;
	VkFence _renderFence;

	VkCommandPool _commandPool;
	VkCommandBuffer _commandBuffer;

	etsutil::DeletionQueue _deletionQueue;

	VkeDescriptorAllocator _descriptorAllocator; // TODO: make growable
};

class EtaRenderer {
public:
	VkResult init(const EtaWindow& window);
	void destroy();

	VkResult drawFrame();
	VkResult endFrame();
};
} // namespace ets
