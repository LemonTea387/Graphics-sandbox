#ifndef INPUT_H
#define INPUT_H

#include "tea/engine_error.hpp"
#include "tea/input/key_code.hpp"
#include "tea/types.hpp"
class Application;

class Input {
 public:
  [[nodiscard]] static Res<Box<Input>, EngineError> create(Application &);
  ~Input() = default;
  Input(const Input &) = delete;
  Input(Input &&) = delete;
  Input &operator=(const Input &) = delete;
  Input &operator=(Input &&) = delete;
  bool is_key_pressed(KeyCode key);

 private:
  Application &m_App;
  Input(Application &);
};

#endif
