#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include <string>

class Window;

struct ApplicationSpec {
  std::string Title;
  std::uint32_t Width;
  std::uint32_t Height;
};

class Application {
 public:
  virtual ~Application() = 0;
  Application() = default;
  Application(const Application &other) = delete;
  Application(const Application &&other) = delete;
  Application &operator=(const Application &other) = delete;
  Application &operator=(const Application &&other) = delete;

  virtual Window &getWindow() = 0;
  virtual void run() = 0;
};

// Cheeky hack for satisfying pure virtual destructors.
inline Application::~Application() {}
#endif
