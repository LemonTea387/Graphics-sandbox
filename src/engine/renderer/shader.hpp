#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
#include <expected>
#include <string>
#include <glm/glm.hpp>
#include "engine/types.hpp"

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

  void setBool(const std::string &name, bool value);
  void setInt(const std::string &name, int value);
  void setIntArray(const std::string &name, int *values, std::uint32_t count);
  void setFloat(const std::string &name, float value);
  void setFloat2(const std::string &name, const glm::vec2 &value);
  void setFloat3(const std::string &name, const glm::vec3 &value);
  void setFloat4(const std::string &name, const glm::vec4 &value);
  void setMat4(const std::string &name, const glm::mat4 &value);

  const std::string &getName() const;

  static RefRes<Shader> create(const std::string &name,
                               const std::string &vertex_path,
                               const std::string &fragment_path);

 private:
  Shader();
  static Res<std::string> readFile(const std::string &filepath);
  std::string m_Name;
  std::uint32_t m_Program;
};

#endif
