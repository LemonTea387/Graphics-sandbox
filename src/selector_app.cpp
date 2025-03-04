#include "selector_app.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <format>
#include <iostream>
#include <memory>

constexpr uint8_t OPENGL_VERSION{41};

std::expected<std::unique_ptr<PlaygroundApp>, EngineError>
PlaygroundApp::create(const ApplicationSpec& spec) {
  auto app = std::unique_ptr<PlaygroundApp>(new PlaygroundApp());

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
    glfwTerminate();
    return std::unexpected(EngineError::GLAD_ERROR);
  }

  return std::move(app);
}

PlaygroundApp::PlaygroundApp() : Application() {}

PlaygroundApp::~PlaygroundApp() { glfwTerminate(); }

Window& PlaygroundApp::getWindow() { return *m_Window; }

void PlaygroundApp::run() {
  auto window = (GLFWwindow*)m_Window->getNativeWindow();

  if (m_ActiveProgram == nullptr) {
    std::cerr << "No active program configured!" << std::endl;
    return;
  }

  m_ActiveProgram->setup();

  // Loop
  while (!glfwWindowShouldClose(window)) {
    m_ActiveProgram->loop();
    m_Window->update();
  }
}

void PlaygroundApp::runProgram(const std::string& name) {
  // Why do we need 2 lookups.
  if (m_Programs.find(name) == m_Programs.end()) {
    std::cerr << std::format("No program {} found.", name);
    return;
  }

  // Cleanup old program
  if (m_ActiveProgram != nullptr) {
    m_ActiveProgram->cleanup();
  }

  m_ActiveProgram = m_Programs.at(name).get();
  m_ActiveProgram->setup();
}

void PlaygroundApp::registerProgram(std::unique_ptr<Program> program) {
  m_Programs.try_emplace(program->getName(), std::move(program));
}
