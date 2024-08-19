#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eta {

class EtaWindow {
public:
	EtaWindow() {};

	VkResult init(const char* title, uint32_t width, uint32_t height) {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		return m_window == nullptr ? VK_ERROR_INITIALIZATION_FAILED : VK_SUCCESS;
	}

	void destroy() {
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	GLFWwindow* getWindow() { return m_window; }

	VkExtent2D getExtent() {
		uint32_t width, height;
		glfwGetFramebufferSize(m_window, (int*)&width, (int*)&height);

		m_windowExtent.width = width;
		m_windowExtent.height = height;

		return m_windowExtent;
	}

	uint32_t width() { return m_windowExtent.width; }
	uint32_t height() { return m_windowExtent.height; }

private:
	VkExtent2D m_windowExtent{700, 800};
	GLFWwindow* m_window;
};

};	// namespace eta
