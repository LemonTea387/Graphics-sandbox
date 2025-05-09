#include <cstdlib>
#include <string>
#include "programs/camera.hpp"
#include "programs/dvd_screensaver.hpp"
#include "programs/triangle.hpp"
#include "programs/textures.hpp"
#include "playground_app.hpp"
#include "tea/application.hpp"
#include "tea/logging.hpp"

int main() {
  Logger::init();

  ApplicationSpec spec{.Title = std::string("Playground!"),
                       .Width = 800,
                       .Height = 600,
                       .Features = FEATURE_WINDOW};
  auto res = PlaygroundApp::create(spec);
  if (!res.has_value()) {
    TEA_ERROR("App creation error");
    std::exit(EXIT_FAILURE);
  }

  auto app = std::move(*res);

  app->register_program(std::make_unique<TriangleProgram>());
  app->register_program(std::make_unique<TexturesProgram>());
  app->register_program(std::make_unique<ScreenSaverProgram>());
  app->run_program("Triangle");
  app->run();

  std::exit(EXIT_SUCCESS);
}
