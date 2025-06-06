#include "tea/input/input.hpp"

#include "tea/application.hpp"
#include "tea/renderer/window.hpp"

#include <GLFW/glfw3.h>

Res<Box<Input>, EngineError> Input::create(Application& app) {
  auto input = Box<Input>(new Input(app));
  return input;
}

Input::Input(Application& app) : m_App{app} {}

bool Input::is_key_pressed(KeyCode key) const {
  auto window =
      (GLFWwindow*)m_App.get_component<Window>()->get().get_native_window();
  int state = glfwGetKey(window, static_cast<int>(key));
  return state == GLFW_PRESS;
}

glm::vec2 Input::get_cursor_pos() const {
  auto window =
      (GLFWwindow*)m_App.get_component<Window>()->get().get_native_window();
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return glm::vec2{xpos, ypos};
}
