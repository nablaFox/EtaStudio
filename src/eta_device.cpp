#include "../include/eta_descriptor.hpp"

using namespace eta;

#include "../include/eta_device.hpp"

using namespace eta;

VkResult EtaDevice::init(const EtaWindow& window) { return VK_SUCCESS; }

void EtaDevice::destroy() {}

AllocatedBuffer EtaDevice::createBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage) {
	// Implementation for creating a buffer

	return {};
}

VkResult EtaDevice::drawFrame() {
	// Implementation for drawing a frame

	return VK_SUCCESS;
}

VkResult EtaDevice::endFrame() {
	// Implementation for ending a frame

	return VK_SUCCESS;
}
