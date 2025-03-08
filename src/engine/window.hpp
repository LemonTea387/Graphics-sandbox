#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <cstdint>
#include <expected>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "engine/engine_error.hpp"
#include "engine/types.hpp"

struct WindowSpec {
  std::string Title{};
  std::uint32_t Width{};
  std::uint32_t Height{};
};

class Window {
 public:
  /*
   * Constructors/Operators
   */
  [[nodiscard]] static RefRes<Window, EngineError> create(const WindowSpec &);
  ~Window();
  Window(const Window &other) = delete;
  Window &operator=(const Window &other) = delete;
  Window(const Window &&other) = delete;
  Window &operator=(const Window &&other) = delete;

  /*
   * Functions
   */
  void update();

  void *getNativeWindow();

 private:
  GLFWwindow *m_Window{nullptr};
  Window();
};
#endif
