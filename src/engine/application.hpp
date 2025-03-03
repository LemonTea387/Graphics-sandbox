#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include <expected>
#include <memory>
#include <string>
#include "engine/error.hpp"
#include "engine/window.hpp"

struct ApplicationSpec {
  std::string Title;
  uint32_t Width;
  uint32_t Height;
};

class Application {
 public:
  static std::expected<std::unique_ptr<Application>, EngineError> create(
      const ApplicationSpec &);
  ~Application();
  Application(const Application &other) = delete;
  Application &operator=(const Application &other) = delete;
  Application(const Application &&other) = delete;
  Application &operator=(const Application &&other) = delete;

  Window &getWindow();

  void run();

 private:
  Application();
  std::unique_ptr<Window> m_Window;
};

#endif
