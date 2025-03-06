#ifndef PROGRAM_TRIANGLE_H
#define PROGRAM_TRIANGLE_H

#include <glad/gl.h>
#include <GL/gl.h>
#include "engine/renderer/shader.hpp"
#include "programs/program.hpp"
class TriangleProgram : public Program {
 public:
  TriangleProgram();
  ~TriangleProgram() override;
  void setup() override;
  void loop() override;
  void cleanup() override;

 private:
  std::unique_ptr<Shader> m_Shader;
  GLuint m_Vao;
  GLuint m_Vbo;
  /// Mark this program as active, will cleanup when destroying if active.
  bool m_Active;
};

#endif
