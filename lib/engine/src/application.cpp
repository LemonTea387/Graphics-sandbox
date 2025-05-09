#include <tea/application.hpp>
#include "tea/engine_error.hpp"
#include "tea/logging.hpp"

constexpr std::uint8_t OPENGL_VERSION{41};

Application::Application() {}
Application::~Application() { glfwTerminate(); }

Res<void, EngineError> init_glfw() {
  int res;
  res = glfwInit();
  if (res == GLFW_FALSE) {
    TEA_CORE_ERROR("GLFW Init error");
    return Err<EngineError>(EngineError::GLFW_ERROR);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION / 10);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION % 10);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return Res<void, EngineError>{};
}

Res<void, EngineError> Application::init_components(
    const ApplicationSpec& spec) {
  TEA_CORE_INFO("Initializing components {}", spec.Features.to_string());
  if (spec.Features.test(0)) {
    TEA_CORE_INFO("Initializing Window");
    auto res = init_glfw();
    if (!res.has_value()) {
      return res;
    }

    WindowSpec window_spec{
        .Title = spec.Title, .Width = spec.Width, .Height = spec.Height};
    auto window = Window::create(window_spec);
    if (!window.has_value()) {
      TEA_CORE_ERROR("Window creation error");
      return Err<EngineError>(window.error());
    }
    this->m_Window = std::move(*window);

    TEA_CORE_INFO("Setting GL context");
    // Set curr gl context
    glfwMakeContextCurrent((GLFWwindow*)this->m_Window->get_native_window());

    TEA_CORE_INFO("Initializing GLAD");
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
      TEA_CORE_ERROR("Failed to initialize GLAD");
      glfwTerminate();
      return Err<EngineError>(EngineError::GLAD_ERROR);
    }

    // Window Input
    TEA_CORE_INFO("Creating Input Component");
    auto input = Input::create(*this);
    if (!input.has_value()) {
      TEA_CORE_ERROR("Input component creation error");
      return Err<EngineError>(input.error());
    }
    this->m_Input = std::move(*input);
  }

  return Res<void, EngineError>();
}

double Application::get_time() const { return glfwGetTime(); }

template <>
Res<Ref<Window>, EngineError> Application::get_component<Window>() {
  if (m_Window) {
    return Res<Ref<Window>, EngineError>{*m_Window};
  }
  return Err<EngineError>(EngineError::COMPONENT_NOT_INITIALIZED);
}
template <>
Res<Ref<Input>, EngineError> Application::get_component<Input>() {
  if (m_Input) {
    return Res<Ref<Input>, EngineError>{*m_Input};
  }
  return Err<EngineError>(EngineError::COMPONENT_NOT_INITIALIZED);
}
