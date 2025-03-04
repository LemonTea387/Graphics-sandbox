#include "programs/triangle.hpp"
#include <glad/gl.h>
#include <GL/gl.h>
#include "engine/renderer/shader.hpp"

void TriangleProgram::setup() {
  float vertices[] = {
      -0.4f, -0.4f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f,
  };
  glGenBuffers(1, &m_Vbo);

  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  auto shader = Shader::create("triangle", "./shaders/triangle.vert.glsl",
                               "./shaders/triangle.frag.glsl");
  if (!shader.has_value()) {
    return;
  }
  m_Shader = std::move(*shader);
  m_Active = true;
}

void TriangleProgram::loop() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  // Use program
  m_Shader->bind();
  glBindVertexArray(m_Vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TriangleProgram::cleanup() {
  glDeleteBuffers(1, &m_Vbo);
  m_Shader.reset();
  m_Active = false;
}

TriangleProgram::TriangleProgram() : Program("Triangle") {}
TriangleProgram::~TriangleProgram() {
  if (m_Active) cleanup();
}
