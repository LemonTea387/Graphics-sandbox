#ifndef PROGRAM_SCREENSAVER_H
#define PROGRAM_SCREENSAVER_H

#include <glad/gl.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "programs/program.hpp"
class Texture;

class Shader;

class ScreenSaverProgram : public Program {
 public:
  ~ScreenSaverProgram() override;
  ScreenSaverProgram();

  Res<void, Error> setup() override;
  void loop() override;
  void cleanup() override;

 private:
  Ref<Shader> m_Shader{nullptr};
  GLuint m_Vao;
  GLuint m_Vbo;
  GLuint m_Ebo;
  Ref<Texture> m_Texture{nullptr};

  glm::vec2 m_Velocity{};
  glm::vec2 m_Offset{};
};

#endif
