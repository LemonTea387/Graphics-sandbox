#ifndef PROGRAM_TEXTURES_H
#define PROGRAM_TEXTURES_H

#include <glad/gl.h>
#include <GL/gl.h>
#include "programs/program.hpp"
#include "tea/types.hpp"

class Texture;

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

  Box<Shader> m_Shader{nullptr};
  GLuint m_Vao;
  GLuint m_Vbo;
  GLuint m_Ebo;
  Box<Texture> m_TextureContainer{nullptr};
  Box<Texture> m_TextureFace{nullptr};
};

#endif
