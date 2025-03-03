#include <glad/gl.h>
#include <cstdint>
#include <iostream>
#include "engine/error.hpp"
#include <GLFW/glfw3.h>
#include "engine/application.hpp"

constexpr uint8_t OPENGL_VERSION{41};

std::expected<std::unique_ptr<Application>, EngineError> Application::create(
    const ApplicationSpec& spec) {
  auto app = std::unique_ptr<Application>(new Application());

  // Create Window
  int res;
  res = glfwInit();
  if (res == GLFW_FALSE) {
    std::cerr << "GLFW Init error" << std::endl;
    return std::unexpected(EngineError::GLFW_ERROR);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION / 10);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION % 10);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  WindowSpec window_spec{
      .Title = spec.Title, .Width = spec.Width, .Height = spec.Height};
  auto window = Window::create(window_spec);
  if (!window.has_value()) {
    std::cerr << "Window creation error" << std::endl;
    return std::unexpected(window.error());
  }
  app->m_Window = std::move(*window);
  // Set curr gl context
  // TODO: Move this somewhere else.
  glfwMakeContextCurrent((GLFWwindow*)app->m_Window->getNativeWindow());

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    std::cerr << "Failed to initialize GLAD";
    std::exit(-1);
  }

  return std::move(app);
}
Application::Application() {}
Application::~Application() { glfwTerminate(); }

// TODO: move somewhere
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
void Application::run() {
  auto window = (GLFWwindow*)m_Window->getNativeWindow();
  // Loop
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    processInput(window);
    m_Window->update();
  }
}
Window& Application::getWindow() { return *m_Window; }
