#include <cstdlib>
#include <iostream>
#include <string>
#include "programs/triangle.hpp"
#include "playground_app.hpp"

int main() {
  ApplicationSpec spec{
      .Title = std::string("Playground!"), .Width = 800, .Height = 600};
  auto res = PlaygroundApp::create(spec);
  if (!res.has_value()) {
    std::cerr << "App creation error" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  auto app = std::move(*res);

  app->registerProgram(std::make_unique<TriangleProgram>());
  app->runProgram("Triangle");
  app->run();

  std::exit(EXIT_SUCCESS);
}
