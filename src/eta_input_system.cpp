#include "../include/eta_input_system.hpp"
#include "../include/eta_scene.hpp"

#include "../include/eta_default_components.hpp"

using namespace eta;

void EtaInputSystem::update(float _) {
	glfwPollEvents();

	if (glfwWindowShouldClose(m_window.getWindow()))
		m_engine.quit = true;

	auto view = currentScene().getEntities<InputComponent>();

	view.each([this](auto entity, InputComponent& input) {
		for (int i = 0; i < GLFW_KEY_LAST; i++) {
			bool currentState = glfwGetKey(m_window.getWindow(), i) == GLFW_PRESS;

			if (currentState && !input.prevKeys[i])
				input.singleKeyPresses[i] = true;
			else
				input.singleKeyPresses[i] = false;

			input.prevKeys[i] = currentState;
			input.keys[i] = currentState;
		}

		glfwGetCursorPos(m_window.getWindow(), &input.mouseX, &input.mouseY);

		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i)
			input.mouseButtons[i] = glfwGetMouseButton(m_window.getWindow(), i) == GLFW_PRESS;
	});
}
