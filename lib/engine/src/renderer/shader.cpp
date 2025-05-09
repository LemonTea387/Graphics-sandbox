#include "tea/renderer/shader.hpp"
#include <glad/gl.h>
#include <GL/gl.h>
#include <glm/ext.hpp>
#include <format>
#include <fstream>
#include <iostream>

void Shader::bind() const { glUseProgram(m_Program); }
void Shader::unbind() const { glUseProgram(0); }
void Shader::set_bool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(m_Program, name.c_str()), (int)value);
}
void Shader::set_int(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}
void Shader::set_int_array(const std::string &name, int *values,
                           std::uint32_t count) const {
  glUniform1iv(glGetUniformLocation(m_Program, name.c_str()), count, values);
}
void Shader::set_float(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}
void Shader::set_float2(const std::string &name, const glm::vec2 &value) const {
  glUniform2f(glGetUniformLocation(m_Program, name.c_str()), value.x, value.y);
}
void Shader::set_float3(const std::string &name, const glm::vec3 &value) const {
  glUniform3f(glGetUniformLocation(m_Program, name.c_str()), value.x, value.y,
              value.z);
}
void Shader::set_float4(const std::string &name, const glm::vec4 &value) const {
  glUniform4f(glGetUniformLocation(m_Program, name.c_str()), value.x, value.y,
              value.z, value.a);
}
void Shader::set_mat4(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

Res<Ref<Shader>, EngineError> Shader::create(const std::string &name,
                                             const std::string &vertex_path,
                                             const std::string &fragment_path) {
  auto shader = std::unique_ptr<Shader>(new Shader());
  shader->m_Name = name;

  // Vertex
  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  auto read_res = read_file(vertex_path);
  if (!read_res.has_value()) {
    return std::unexpected(read_res.error());
  }
  std::string vertex_source{*read_res};
  const char *temp = vertex_source.c_str();
  glShaderSource(vertex, 1, &temp, NULL);
  glCompileShader(vertex);

  int success;
  char infoLog[512];
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cerr << std::format("Error compiling vertex shader {}", infoLog)
              << std::endl;
    return std::unexpected(EngineError::SHADER_ERROR);
  }

  // Fragment
  GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
  read_res = read_file(fragment_path);
  if (!read_res.has_value()) {
    return std::unexpected(read_res.error());
  }
  std::string fragment_source{*read_res};
  temp = fragment_source.c_str();
  glShaderSource(fragment, 1, &temp, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cerr << std::format("Error compiling fragment shader {}", infoLog)
              << std::endl;
    glDeleteShader(vertex);
    return std::unexpected(EngineError::SHADER_ERROR);
  }

  // Link
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cerr << std::format("Error linking shader program {}", infoLog)
              << std::endl;
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return std::unexpected(EngineError::SHADER_ERROR);
  }
  shader->m_Program = program;

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return shader;
}

Res<std::string, EngineError> Shader::read_file(const std::string &filepath) {
  std::string res;
  std::ifstream in(filepath, std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    std::ifstream::pos_type size = in.tellg();
    if (size != -1) {
      res.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&res[0], size);
    } else {
      std::cerr << std::format("Could not read file {}", filepath) << std::endl;
      return std::unexpected(EngineError::FILE_IO_ERROR);
    }
  } else {
    std::cerr << std::format("Could not open file {}", filepath) << std::endl;
    return std::unexpected(EngineError::FILE_IO_ERROR);
  }

  return res;
}
const std::string &Shader::get_name() const { return m_Name; }

Shader::Shader() {}
Shader::~Shader() {
  if (m_Program != 0) glDeleteProgram(m_Program);
}
