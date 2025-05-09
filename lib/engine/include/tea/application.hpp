#ifndef APPLICATION_H
#define APPLICATION_H

#include <bitset>
#include <cstdint>
#include <string>
#include "tea/engine_error.hpp"
#include "tea/input/input.hpp"
#include "tea/renderer/window.hpp"

#define FEATURE_WINDOW 1 << 0

struct ApplicationSpec {
  std::string Title;
  std::uint32_t Width;
  std::uint32_t Height;
  std::bitset<8> Features;
};

class Application {
 public:
  virtual void run() = 0;
  virtual ~Application() = 0;
  Application();
  Application(const Application &other) = delete;
  Application(const Application &&other) = delete;
  Application &operator=(const Application &other) = delete;
  Application &operator=(const Application &&other) = delete;

  [[nodiscard]] Res<void, EngineError> init_components(const ApplicationSpec &);

  double get_time() const;
  // TODO: Have a better system than this
  template <class T>
  Res<Ref<T>, EngineError> get_component();

 private:
  Box<Window> m_Window{nullptr};
  Box<Input> m_Input{nullptr};

  ApplicationSpec m_Spec;
};
#endif
