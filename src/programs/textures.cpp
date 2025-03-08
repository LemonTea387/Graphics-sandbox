#include "programs/textures.hpp"
#include <cstdint>
#include <iostream>

#include "engine/renderer/shader.hpp"
#include "programs/triangle.hpp"
#include "stb_image.h"

Res<void, Error> TexturesProgram::setup() {
  std::int32_t width, height, nrChannels;
  std::uint8_t *data =
      stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
  if (data == nullptr) {
    std::cerr << "Failed to load image" << std::endl;
    return std::unexpected(Error::IO_ERROR);
  }

  glGenTextures(1, &m_Texture);
  glBindTexture(GL_TEXTURE_2D, m_Texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // Free image
  stbi_image_free(data);

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
    return std::unexpected(Error::PROGRAM_ERROR);
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

  glBindTexture(GL_TEXTURE_2D, m_Texture);

  glBindVertexArray(m_Vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  m_Shader->unbind();
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TexturesProgram::cleanup() {
  glDeleteBuffers(1, &m_Vbo);
  glDeleteBuffers(1, &m_Ebo);
  glDeleteVertexArrays(1, &m_Vao);
  glDeleteTextures(1, &m_Texture);
  m_Shader.reset();
  m_Active = false;
}

TexturesProgram::TexturesProgram()
    : Program("Textures", "Idk like load textures?") {}
TexturesProgram::~TexturesProgram() {
  if (m_Active) cleanup();
}
