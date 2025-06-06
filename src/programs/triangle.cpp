#include "programs/triangle.hpp"
#include "tea/renderer/shader.hpp"

Res<void, Error> TriangleProgram::setup() {
  float vertices[] = {
      -0.4f, -0.4f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f,
  };
  glGenBuffers(1, &m_Vbo);

  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  auto shader = Shader::create("triangle", "./shaders/triangle.vert.glsl",
                               "./shaders/triangle.frag.glsl");
  if (!shader.has_value()) {
    return std::unexpected(Error::PROGRAM_ERROR);
  }
  m_Shader = std::move(*shader);
  m_Active = true;

  // What is this
  return Res<void, Error>{};
}

void TriangleProgram::loop(Application&) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  // Use program
  m_Shader->bind();
  glBindVertexArray(m_Vao);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  m_Shader->unbind();
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleProgram::cleanup() {
  glDeleteBuffers(1, &m_Vbo);
  glDeleteVertexArrays(1, &m_Vao);
  m_Shader.reset();
  m_Active = false;
}

TriangleProgram::TriangleProgram()
    : Program("Triangle", "Stupid program to display a triangle.") {}
TriangleProgram::~TriangleProgram() {
  if (m_Active) cleanup();
}
