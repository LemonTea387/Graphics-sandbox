#ifndef APP_H
#define APP_H

#include <unordered_map>
#include "engine/application.hpp"
#include "engine/window.hpp"
#include "programs/program.hpp"
class PlaygroundApp : public Application {
 public:
  ~PlaygroundApp() override;
  static std::expected<std::unique_ptr<PlaygroundApp>, EngineError> create(
      const ApplicationSpec &);
  Window &getWindow() override;
  void run() override;

  /**
   * Function to swap out the program to run.
   * Immediately stops the current program, performs cleanup and loop the new
   * one.
   * @note Should be called within the run loop.
   */
  void runProgram(const std::string &name);
  void registerProgram(std::unique_ptr<Program> program);

 private:
  PlaygroundApp();
  Program *m_ActiveProgram;
  std::unordered_map<std::string, std::unique_ptr<Program>> m_Programs;
  std::unique_ptr<Window> m_Window;
};

#endif
