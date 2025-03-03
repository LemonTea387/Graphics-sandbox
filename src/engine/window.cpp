#include "engine/window.hpp"
#include <iostream>
#include <memory>
#include "GLFW/glfw3.h"
#include "engine/error.hpp"

void framebufSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

std::expected<std::unique_ptr<Window>, EngineError> Window::create(
    const WindowSpec& spec) {
  bool res;
  auto rw = std::unique_ptr<Window>(new Window());

  GLFWwindow* window =
      glfwCreateWindow(spec.Width, spec.Height, spec.Title.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window";
    return std::unexpected(EngineError::GLFW_ERROR);
  }
  rw->m_Window = window;
  // Handle resize events
  glfwSetFramebufferSizeCallback(window, framebufSizeCallback);

  return rw;
}

void Window::update() {
  glfwSwapBuffers(m_Window);
  glfwPollEvents();
}

void* Window::getNativeWindow() { return m_Window; }

Window::Window() {}
Window::~Window() {}
