#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include <memory>
#include <expected>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "engine/error.hpp"

struct WindowSpec {
  std::string Title;
  uint32_t Width;
  uint32_t Height;
};

class Window {
 public:
  /*
   * Constructors/Operators
   */
  static std::expected<std::unique_ptr<Window>, EngineError> create(
      const WindowSpec &);
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
  GLFWwindow *m_Window;
  Window();
};
#endif
