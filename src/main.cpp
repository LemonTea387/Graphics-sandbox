#include <iostream>
#include <string>
#include "engine/application.hpp"

int main(int argc, char** argv) {
  ApplicationSpec spec{
      .Title = std::string("Playground!"), .Width = 800, .Height = 600};
  auto app = Application::create(spec);

  if (!app.has_value()) {
    std::cerr << "App creation error" << std::endl;
    std::exit(1);
  }

  (*app)->run();

  std::exit(0);
}
