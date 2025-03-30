#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <expected>
#include <string>
#include <glm/glm.hpp>
#include "tea/types.hpp"
#include "tea/engine_error.hpp"

class Shader {
 public:
  ~Shader();
  Shader(const Shader &other) = delete;
  Shader &operator=(const Shader &other) = delete;
  Shader(const Shader &&other) = delete;
  Shader &operator=(const Shader &&other) = delete;

  /**
   * Binds the shader program.
   */
  void bind() const;
  void unbind() const;

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setIntArray(const std::string &name, int *values,
                   std::uint32_t count) const;
  void setFloat(const std::string &name, float value) const;
  void setFloat2(const std::string &name, const glm::vec2 &value) const;
  void setFloat3(const std::string &name, const glm::vec3 &value) const;
  void setFloat4(const std::string &name, const glm::vec4 &value) const;
  void setMat4(const std::string &name, const glm::mat4 &value) const;

  const std::string &getName() const;

  [[nodiscard]] static Res<Ref<Shader>, EngineError> create(
      const std::string &name, const std::string &vertex_path,
      const std::string &fragment_path);

 private:
  Shader();
  [[nodiscard]] static Res<std::string, EngineError> readFile(
      const std::string &filepath);
  std::string m_Name;
  std::uint32_t m_Program;
};

#endif
