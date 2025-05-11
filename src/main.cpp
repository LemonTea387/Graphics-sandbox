#include <cstdlib>
#include "programs/camera.hpp"
#include "programs/dvd_screensaver.hpp"
#include "programs/triangle.hpp"
#include "programs/textures.hpp"
#include "playground_app.hpp"
#include "tea/logging.hpp"

int main() {
  Logger::init();

  PlaygroundApp app;

  app.register_program(std::make_unique<TriangleProgram>());
  app.register_program(std::make_unique<TexturesProgram>());
  app.register_program(std::make_unique<ScreenSaverProgram>());
  app.run_program("Triangle");
  app.run();

  std::exit(EXIT_SUCCESS);
}
