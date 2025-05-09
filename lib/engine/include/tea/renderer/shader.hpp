#ifndef SHADER_H
#define SHADER_H

#include <cstdint>
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

  void set_bool(const std::string &name, bool value) const;
  void set_int(const std::string &name, int value) const;
  void set_int_array(const std::string &name, int *values,
                     std::uint32_t count) const;
  void set_float(const std::string &name, float value) const;
  void set_float2(const std::string &name, const glm::vec2 &value) const;
  void set_float3(const std::string &name, const glm::vec3 &value) const;
  void set_float4(const std::string &name, const glm::vec4 &value) const;
  void set_mat4(const std::string &name, const glm::mat4 &value) const;

  const std::string &get_name() const;

  [[nodiscard]] static Res<Box<Shader>, EngineError> create(
      const std::string &name, const std::string &vertex_path,
      const std::string &fragment_path);

 private:
  Shader();
  [[nodiscard]] static Res<std::string, EngineError> read_file(
      const std::string &filepath);
  std::string m_Name;
  std::uint32_t m_Program;
};

#endif
