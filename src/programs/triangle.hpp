#ifndef PROGRAM_TRIANGLE_H
#define PROGRAM_TRIANGLE_H

#include <glad/gl.h>
#include <GL/gl.h>
#include <memory>
#include "programs/program.hpp"

class Shader;

class TriangleProgram : public Program {
 public:
  TriangleProgram();
  ~TriangleProgram() override;
  Res<void, Error> setup() override;
  void loop(Application&) override;
  void cleanup() override;

 private:
  std::unique_ptr<Shader> m_Shader;
  GLuint m_Vao;
  GLuint m_Vbo;
  /// Mark this program as active, will cleanup when destroying if active.
  bool m_Active;
};

#endif
