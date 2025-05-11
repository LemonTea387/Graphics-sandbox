#include "programs/dvd_screensaver.hpp"
#include <cstdlib>
#include "tea/logging.hpp"
#include "tea/renderer/texture.hpp"
#include "tea/renderer/shader.hpp"

ScreenSaverProgram::ScreenSaverProgram()
    : Program("Dvd Screen saver",
              "The thing we all like to see if it hits the corner or not.") {}
ScreenSaverProgram::~ScreenSaverProgram() { cleanup(); }

Res<void, Error> ScreenSaverProgram::setup() {
  float vertices[] = {
      // positions        // texture coords
      0.2f,  0.15f,  0.0f, 1.0f, 1.0f,  // top right
      0.2f,  -0.15f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.2f, -0.15f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.2f, 0.15f,  0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  // Prep buffers
  glGenBuffers(1, &m_Vbo);
  glGenBuffers(1, &m_Ebo);
  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // For layout 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // layout 1
  // Texture coords
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  auto res = Texture::create("assets/DVD_logo.png", Renderer::ImageMode::RGBA);
  if (!res.has_value()) {
    TEA_ERROR("Error creating dvd texture");
    return Err(Error::ENGINE_ERROR);
  }
  m_Texture = std::move(*res);

  auto shader_res = Shader::create("dvd", "./shaders/dvd_screensaver.vert.glsl",
                                   "./shaders/dvd_screensaver.frag.glsl");
  if (!shader_res.has_value()) {
    TEA_ERROR("Error creating dvd shader");
    return Err(Error::ENGINE_ERROR);
  }
  m_Shader = std::move(*shader_res);
  m_Shader->bind();
  m_Shader->set_float3("color", glm::vec3((double)rand() / (RAND_MAX),
                                          ((double)rand() / RAND_MAX),
                                          ((double)rand() / RAND_MAX)));
  m_Shader->unbind();

  m_Velocity = {0.005, 0.005};

  return Res<void, Error>{};
}

void ScreenSaverProgram::loop(Application&) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_Shader->bind();
  m_Offset += m_Velocity;
  if (m_Offset.x >= 0.8 || m_Offset.x <= -0.8) {
    m_Velocity.x = -m_Velocity.x;
    m_Shader->set_float3("color", glm::vec3((double)rand() / (RAND_MAX),
                                            ((double)rand() / RAND_MAX),
                                            ((double)rand() / RAND_MAX)));
  }
  if (m_Offset.y >= 0.85 || m_Offset.y <= -0.85) {
    m_Velocity.y = -m_Velocity.y;
    m_Shader->set_float3("color", glm::vec3((double)rand() / (RAND_MAX),
                                            ((double)rand() / RAND_MAX),
                                            ((double)rand() / RAND_MAX)));
  }

  auto _ = m_Texture->bind(0);
  m_Shader->set_int("uniform_texture1", 0);
  m_Shader->set_float2("offset", m_Offset);

  glBindVertexArray(m_Vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  m_Shader->unbind();
  glBindVertexArray(0);
  glDisableVertexAttribArray(0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void ScreenSaverProgram::cleanup() {
  glDeleteVertexArrays(1, &m_Vao);
  glDeleteBuffers(1, &m_Vbo);
  glDeleteBuffers(1, &m_Ebo);
}
