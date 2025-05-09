#include "playground_app.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "tea/logging.hpp"
#include "tea/renderer/window.hpp"
#include "programs/program.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace DebugMenu {
constexpr ImVec2 WINDOW_SIZE{500, 440};
constexpr ImVec2 LIST_SIZE{150, 0};
};  // namespace DebugMenu

Res<Box<PlaygroundApp>, Error> PlaygroundApp::create(
    const ApplicationSpec& spec) {
  auto app = Box<PlaygroundApp>(new PlaygroundApp());
  auto res = app->init_components(spec);
  if (!res.has_value()) {
    TEA_ERROR("Error initializing components for application");
    return Err<Error>(Error::ENGINE_ERROR);
  }

  TEA_INFO("Initializing IMGUI");
  // TODO: maybe move somewhere?
  // imgui State
  app->m_State = {};
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      (GLFWwindow*)app->get_component<Window>()->get().get_native_window(),
      true);  // Second param install_callback=true will install
              // GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  return app;
}

PlaygroundApp::PlaygroundApp() : Application() {}

PlaygroundApp::~PlaygroundApp() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void PlaygroundApp::run() {
  if (m_ActiveProgram == nullptr) {
    TEA_ERROR("No active program configured");
    return;
  }

  // Loop
  auto window =
      (GLFWwindow*)this->get_component<Window>()->get().get_native_window();
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    showProgramSelector();
    // Handle some state after imgui
    // TODO: Make it event based
    if (m_State.Run && m_State.SelectedProgram != nullptr) {
      // Yes this is kinda dumb
      run_program(m_State.SelectedProgram->getName());
      m_State.Run = false;
    }

    m_ActiveProgram->loop();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    this->get_component<Window>()->get().update();
  }
}

void PlaygroundApp::run_program(const std::string& name) {
  if (auto program = m_Programs.find(name); program == m_Programs.end()) {
    TEA_ERROR("No program {} found", name);
    return;
  }

  // Cleanup old program
  if (m_ActiveProgram != nullptr) {
    m_ActiveProgram->cleanup();
  }

  // TODO: Handle this error
  m_ActiveProgram = m_Programs.at(name).get();
  m_ActiveProgram->setup();
}

void PlaygroundApp::register_program(std::unique_ptr<Program> program) {
  m_Programs.try_emplace(program->getName(), std::move(program));
}

void PlaygroundApp::showProgramSelector() {
  ImGui::SetNextWindowSize(DebugMenu::WINDOW_SIZE, ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Pick your poison.", nullptr, ImGuiWindowFlags_None)) {
    ImGui::Text("Currently Active : %s",
                m_ActiveProgram != nullptr ? m_ActiveProgram->getName().c_str()
                                           : "None");
    // Programs
    {
      ImGui::BeginChild("Programs", DebugMenu::LIST_SIZE,
                        ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

      // Some c++17 arcane shit that I just discovered.
      for (auto&& [name, program] : m_Programs) {
        if (ImGui::Selectable(name.c_str(),
                              m_State.SelectedProgram == program.get())) {
          m_State.SelectedProgram = program.get();
        }
      }
      ImGui::EndChild();
    }
    ImGui::SameLine();

    // Description
    {
      ImGui::BeginGroup();
      ImGui::BeginChild("Details",
                        ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
      if (m_State.SelectedProgram != nullptr) {
        ImGui::Text("%s", m_State.SelectedProgram->getName().c_str());
      } else {
        ImGui::Text("Select a Program!");
      }
      ImGui::Separator();
      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Description")) {
          if (m_State.SelectedProgram != nullptr) {
            ImGui::TextWrapped(
                "%s", m_State.SelectedProgram->getDescription().c_str());
          } else {
            ImGui::Text("Select a Program!");
          }
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
      ImGui::EndChild();
      if (ImGui::Button("Load")) {
        m_State.Run = true;
      }
      ImGui::EndGroup();
    }
  }
  ImGui::End();
}
