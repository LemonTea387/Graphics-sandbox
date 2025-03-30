#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <expected>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "tea/engine_error.hpp"
#include "tea/types.hpp"

struct WindowSpec {
  std::string Title{};
  std::uint32_t Width{};
  std::uint32_t Height{};
};

class Window {
 public:
  [[nodiscard]] static Res<Ref<Window>, EngineError> create(const WindowSpec &);
  ~Window();
  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;
  Window(const Window &&other) = delete;
  Window &operator=(const Window &&other) = delete;

  void update();

  void *getNativeWindow();

 private:
  GLFWwindow *m_Window{nullptr};
  Window();
};
#endif
