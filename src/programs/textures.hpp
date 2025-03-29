#ifndef PROGRAM_TEXTURES_H
#define PROGRAM_TEXTURES_H

#include <glad/gl.h>
#include <GL/gl.h>
#include "programs/program.hpp"
#include "tea/types.hpp"

class Shader;

class TexturesProgram : public Program {
 public:
  TexturesProgram();
  ~TexturesProgram() override;
  Res<void, Error> setup() override;
  void loop() override;
  void cleanup() override;

 private:
  /// TODO: Really, draft out some better system
  bool m_Active;

  Ref<Shader> m_Shader;
  GLuint m_Vao;
  GLuint m_Vbo;
  GLuint m_Ebo;
  GLuint m_TextureContainer;
  GLuint m_TextureFace;
};

#endif
