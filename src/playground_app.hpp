#ifndef APP_H
#define APP_H

#include <expected>
#include <unordered_map>

#include "tea/application.hpp"
#include "tea/types.hpp"

class Program;
class Window;
enum class EngineError;

/**
 * Mainly used for bridge of IMGUI with main app.
 * TODO: Will change to events system to drive the program when that's up T_T
 */
struct ProgramState {
  Program *SelectedProgram{nullptr};
  /// A flag to trigger reloading of program based on currently selected
  /// program.
  bool Run{false};
};

class PlaygroundApp : public Application {
 public:
  ~PlaygroundApp() override;
  static Res<Ref<PlaygroundApp>, EngineError> create(const ApplicationSpec &);
  Window &getWindow() override;
  void run() override;

  /**
   * Function to swap out the program to run.
   * Immediately stops the current program, performs cleanup and loop the new
   * one.
   * @note Should be called within the run loop.
   */
  void runProgram(const std::string &name);
  void registerProgram(Ref<Program> program);

 private:
  PlaygroundApp();
  Program *m_ActiveProgram{nullptr};
  std::unordered_map<std::string, Ref<Program>> m_Programs{};
  Ref<Window> m_Window;

  // TODO: Maybe should move it out?
  ProgramState m_State{};
  void showProgramSelector();
};

#endif
