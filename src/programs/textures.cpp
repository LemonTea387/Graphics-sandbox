#include "programs/textures.hpp"
#include <cstdint>
#include "tea/logging.hpp"
#include "tea/renderer/shader.hpp"
#include "tea/renderer/texture.hpp"
#include "error.hpp"
#include "programs/triangle.hpp"

Res<void, Error> TexturesProgram::setup() {
  auto res = Texture::create("assets/container.jpg", Renderer::ImageMode::RGB);
  if (!res.has_value()) {
    TEA_ERROR("Error creating container texture");
    return Err(Error::ENGINE_ERROR);
  }
  m_TextureContainer = std::move(*res);

  res = Texture::create("assets/awesomeface.png", Renderer::ImageMode::RGBA);
  if (!res.has_value()) {
    TEA_ERROR("Error creating awesomeface texture");
    return Err(Error::ENGINE_ERROR);
  }
  m_TextureFace = std::move(*res);

  // Draw manually
  float vertices[]{
      // positions        // colors         // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
  };
  std::uint32_t indices[]{
      3, 0, 2,  // top left, top right, btm left
      0, 1, 2   // Top right, btm right, btm left
  };
  glGenVertexArrays(1, &m_Vao);
  glGenBuffers(1, &m_Vbo);
  glGenBuffers(1, &m_Ebo);

  glBindVertexArray(m_Vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // For layout 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // layout 1
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // layout 2
  // Texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  auto shader = Shader::create("textures", "./shaders/textures.vert.glsl",
                               "./shaders/textures.frag.glsl");
  if (!shader.has_value()) {
    return Err(Error::PROGRAM_ERROR);
  }
  m_Shader = std::move(*shader);
  m_Active = true;

  glBindVertexArray(0);

  return Res<void, Error>{};
}

void TexturesProgram::loop() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_Shader->bind();
  auto _ = m_TextureContainer->bind(0);
  _ = m_TextureFace->bind(1);

  // Set which texture unit belongs to which uniform
  m_Shader->set_int("uniform_texture1", 0);
  m_Shader->set_int("uniform_texture2", 1);

  glBindVertexArray(m_Vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  m_Shader->unbind();
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TexturesProgram::cleanup() {
  glDeleteBuffers(1, &m_Vbo);
  glDeleteBuffers(1, &m_Ebo);
  glDeleteVertexArrays(1, &m_Vao);
  m_TextureContainer.reset();
  m_TextureFace.reset();
  m_Shader.reset();
  m_Active = false;
}

TexturesProgram::TexturesProgram()
    : Program("Textures", "Idk like load textures?") {}
TexturesProgram::~TexturesProgram() {
  if (m_Active) cleanup();
}
