#include "tea/renderer/texture.hpp"

#include <cstdint>
#include "tea/engine_error.hpp"
#include "tea/logging.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

constexpr std::uint32_t BASE_TEXTURE_ADDR = GL_TEXTURE0;

Res<Box<Texture>, EngineError> Texture::create(const std::string &path,
                                               Renderer::ImageMode mode) {
  auto created = new Texture();

  std::int32_t width, height, nrChannels;
  // Images are flipped since opengl expects y0.0 to be bottom.
  stbi_set_flip_vertically_on_load(true);
  std::uint8_t *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data == nullptr) {
    TEA_CORE_ERROR("Failed to load image {}", path);
    return Err(EngineError::FILE_IO_ERROR);
  }
  glGenTextures(1, &created->m_TextureId);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, created->m_TextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, mode.to_gl(),
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

  return Box<Texture>(created);
}

Res<void, EngineError> Texture::bind(std::uint32_t slot) {
  if (slot > 31) {
    TEA_CORE_ERROR("Binding to slot greater than 32");
    return Err(EngineError::TEXTURE_ERROR);
  }
  glActiveTexture(BASE_TEXTURE_ADDR + slot);
  glBindTexture(GL_TEXTURE_2D, m_TextureId);

  return Res<void, EngineError>{};
}

Texture::Texture() {}
Texture::~Texture() { glDeleteTextures(1, &m_TextureId); }
