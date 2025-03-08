#include "playground_app.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <format>
#include <iostream>
#include "engine/window.hpp"
#include "programs/program.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

constexpr std::uint8_t OPENGL_VERSION{41};

std::expected<std::unique_ptr<PlaygroundApp>, EngineError>
PlaygroundApp::create(const ApplicationSpec& spec) {
  auto app = std::unique_ptr<PlaygroundApp>(new PlaygroundApp());

  // Create Window
  int res;
  res = glfwInit();
  if (res == GLFW_FALSE) {
    std::cerr << "GLFW Init error" << std::endl;
    return std::unexpected(EngineError::GLFW_ERROR);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION / 10);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION % 10);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  WindowSpec window_spec{
      .Title = spec.Title, .Width = spec.Width, .Height = spec.Height};
  auto window = Window::create(window_spec);
  if (!window.has_value()) {
    std::cerr << "Window creation error" << std::endl;
    return std::unexpected(window.error());
  }
  app->m_Window = std::move(*window);
  // Set curr gl context
  // TODO: Move this somewhere else.
  glfwMakeContextCurrent((GLFWwindow*)app->m_Window->getNativeWindow());

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    std::cerr << "Failed to initialize GLAD";
    glfwTerminate();
    return std::unexpected(EngineError::GLAD_ERROR);
  }

  // TODO: maybe move somewhere?
  // imgui
  // State
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
      (GLFWwindow*)app->m_Window->getNativeWindow(),
      true);  // Second param install_callback=true will install
              // GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  return std::move(app);
}

PlaygroundApp::PlaygroundApp() : Application() {}

PlaygroundApp::~PlaygroundApp() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
}

Window& PlaygroundApp::getWindow() { return *m_Window; }

void PlaygroundApp::run() {
  auto window = (GLFWwindow*)m_Window->getNativeWindow();

  if (m_ActiveProgram == nullptr) {
    std::cerr << "No active program configured!" << std::endl;
    return;
  }

  m_ActiveProgram->setup();

  // Loop
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
      runProgram(m_State.SelectedProgram->getName());
      m_State.Run = false;
    }

    m_ActiveProgram->loop();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_Window->update();
  }
}

void PlaygroundApp::runProgram(const std::string& name) {
  if (auto program = m_Programs.find(name); program == m_Programs.end()) {
    std::cerr << std::format("No program {} found.", name);
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

void PlaygroundApp::registerProgram(std::unique_ptr<Program> program) {
  m_Programs.try_emplace(program->getName(), std::move(program));
}

void PlaygroundApp::showProgramSelector() {
  ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
  if (ImGui::Begin("Pick your poison.", nullptr, ImGuiWindowFlags_None)) {
    ImGui::Text("Currently Active : %s",
                m_ActiveProgram != nullptr ? m_ActiveProgram->getName().c_str()
                                           : "None");
    // Programs
    {
      ImGui::BeginChild("Programs", ImVec2(150, 0),
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
      ImGui::BeginChild(
          "Details",
          ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));  // Leave room for 1
                                                            // line below us
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
