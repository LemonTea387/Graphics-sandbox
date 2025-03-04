#include "engine/renderer/shader.hpp"
#include <glad/gl.h>
#include <GL/gl.h>
#include <format>
#include <fstream>
#include <iostream>
#include "engine/error.hpp"

void Shader::bind() const { glUseProgram(m_Program); }
void Shader::unbind() const { glUseProgram(0); }
void Shader::setInt(const std::string &name, int value) {}
void Shader::setIntArray(const std::string &name, int *values, uint32_t count) {
}
void Shader::setFloat(const std::string &name, float value) {}
void Shader::setFloat2(const std::string &name, const glm::vec2 &value) {}
void Shader::setFloat3(const std::string &name, const glm::vec3 &value) {}
void Shader::setFloat4(const std::string &name, const glm::vec4 &value) {}
void Shader::setMat4(const std::string &name, const glm::mat4 &value) {}

std::expected<std::unique_ptr<Shader>, EngineError> Shader::create(
    const std::string &name, const std::string &vertex_path,
    const std::string &fragment_path) {
  auto shader = std::unique_ptr<Shader>(new Shader());
  // Vertex
  GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
  auto read_res = readFile(vertex_path);
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
  read_res = readFile(fragment_path);
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

std::expected<std::string, EngineError> Shader::readFile(
    const std::string &filepath) {
  std::string res;
  std::ifstream in(filepath, std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
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
const std::string &Shader::getName() const { return m_Name; }

Shader::Shader() {}
Shader::~Shader() {
  if (m_Program != 0) glDeleteProgram(m_Program);
}
