#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/gl.h>
#include "tea/engine_error.hpp"
#include "tea/types.hpp"

namespace Renderer {
class ImageMode {
 public:
  enum Enum { RGB, RGBA };

  constexpr ImageMode() = default;

  constexpr ImageMode(const Enum &e) : m_E(e) {}
  constexpr ~ImageMode() = default;
  ImageMode &operator=(const ImageMode &im) = default;

  // Allows comparisons with Enum constants
  constexpr operator Enum() const { return m_E; }
  explicit operator bool() const = delete;
  GLenum to_gl() {
    switch (m_E) {
      case RGB:
        return GL_RGB;
      case RGBA:
        return GL_RGBA;
      default:
        return GL_RGB;
    }
  }

 private:
  Enum m_E;
};

}  // namespace Renderer

class Texture {
 public:
  ~Texture();
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;
  Texture(const Texture &&) = delete;
  Texture &operator=(const Texture &&) = delete;

  [[nodiscard]]
  static Res<Ref<Texture>, EngineError> create(const std::string &path,
                                               Renderer::ImageMode mode);
  /// slot is 0-based
  Res<void, EngineError> bind(std::uint32_t slot);

 private:
  Texture();
  GLuint m_TextureId{};
};

#endif
