#pragma once

#include "eta_pch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eta {

class EtaWindow {
public:
	EtaWindow(){};

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

	VkExtent2D getExtent() { return m_windowExtent; }

private:
	VkExtent2D m_windowExtent{700, 800};
	GLFWwindow* m_window;
};

}; // namespace eta
