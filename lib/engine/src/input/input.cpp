#include "tea/input/input.hpp"

#include "tea/application.hpp"
#include "tea/window.hpp"

#include <GLFW/glfw3.h>

namespace Input {
bool Input::isKeyPressed(KeyCode key) {
  auto window = (GLFWwindow*)Application::Get().getWindow().getNativeWindow();
  int state = glfwGetKey(window, static_cast<int>(key));
  return state == GLFW_PRESS;
}
}  // namespace Input
