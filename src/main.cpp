#include <iostream>
#include <string>
#include "programs/triangle.hpp"
#include "selector_app.hpp"

int main(int argc, char** argv) {
  ApplicationSpec spec{
      .Title = std::string("Playground!"), .Width = 800, .Height = 600};
  auto res = PlaygroundApp::create(spec);
  if (!res.has_value()) {
    std::cerr << "App creation error" << std::endl;
    std::exit(1);
  }

  auto app = std::move(*res);

  app->registerProgram(std::make_unique<TriangleProgram>());
  app->runProgram("Triangle");
  app->run();

  std::exit(0);
}
