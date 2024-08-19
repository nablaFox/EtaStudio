#pragma once

#include "eta_pch.hpp"

namespace eta {

class EtaDevice;

class EtaSwapchain {
public:
	EtaSwapchain(EtaDevice& device) : m_device(device) {}
	void init(VkExtent2D extent, VkFormat format);
	void destroy();

	VulkanImage& getCurrentImage() { return m_swapchainImages[m_currentImage]; }
	VkExtent2D getExtent() { return m_swapchainExtent; }

	VkResult presentOnScreen(VkSemaphore semaphore = nullptr);
	VkResult acquireImage(VkSemaphore semaphore = nullptr);

	VkSwapchainKHR& get_swapchain() { return m_swapchain; }

private:
	EtaDevice& m_device;

	VkSwapchainKHR m_swapchain;
	VkFormat m_swapchainImageFormat;
	VkExtent2D m_swapchainExtent;

	std::vector<VulkanImage> m_swapchainImages;

	VkPresentInfoKHR m_presentInfo{};
	uint32_t m_currentImage;
};

}  // namespace eta
