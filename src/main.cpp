#include <cstdlib>
#include <string>
#include "programs/dvd_screensaver.hpp"
#include "programs/triangle.hpp"
#include "programs/textures.hpp"
#include "playground_app.hpp"
#include "tea/logging.hpp"

int main() {
  Logger::init();

  ApplicationSpec spec{
      .Title = std::string("Playground!"), .Width = 800, .Height = 600};
  auto res = PlaygroundApp::create(spec);
  if (!res.has_value()) {
    TEA_ERROR("App creation error");
    std::exit(EXIT_FAILURE);
  }

  auto app = std::move(*res);

  app->registerProgram(std::make_unique<TriangleProgram>());
  app->registerProgram(std::make_unique<TexturesProgram>());
  app->registerProgram(std::make_unique<ScreenSaverProgram>());
  app->runProgram("Triangle");
  app->run();

  std::exit(EXIT_SUCCESS);
}
