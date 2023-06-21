#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 Pos;\n"
    "layout (location = 1) in vec2 TexCoord;\n"
    "uniform mat4 transform;\n"
    "out vec2 passedTexCoord;\n"
    "void main() {\n"
    "gl_Position = transform * vec4(Pos, 1.0);\n"
    "passedTexCoord = TexCoord;\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec2 passedTexCoord;\n"
    "uniform sampler2D Texture1;\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "FragColor = texture(Texture1, passedTexCoord);\n"
    "}\0";

enum ASSET_TYPE { JPG, PNG };

const int WIDTH = 800, HEIGHT = 600;

GLFWwindow* initGLFWwindow();
unsigned int compileShaderProgram();
void processInput(GLFWwindow* window);
void framebufSizeCallback(GLFWwindow* window, int width, int height);
unsigned int loadTextures(const char* path, ASSET_TYPE type);

int main(int argc, char** argv) {
  GLFWwindow* window = initGLFWwindow();
  // We also define colors this time
  float vertices[] = {
      // positions        // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
  };
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  // Prep buffers
  unsigned int VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

  // Shaders
  unsigned int shaderProgram = compileShaderProgram();

  // Load textures
  // Images usually have 0.0 at top of y-axis, but OpenGL expects 0.0 at bottom
  stbi_set_flip_vertically_on_load(true);
  unsigned int texture1 = loadTextures("assets/container.jpg", ASSET_TYPE::JPG);

  // Binding textures to corresponding texture unit
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glUniform1i(glGetUniformLocation(shaderProgram, "Texture1"), 0);

  unsigned int transformLocation =
      glGetUniformLocation(shaderProgram, "transform");

  glUseProgram(shaderProgram);
  // Main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Make identity matrix
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(),
                            glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE,
                       glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  // Delete everything
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

GLFWwindow* initGLFWwindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window =
      glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Sandbox", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    std::exit(-1);
  }
  glfwMakeContextCurrent(window);

  // Init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    std::exit(-1);
  }

  // Set Viewport
  glViewport(0, 0, WIDTH, HEIGHT);
  // Handle resize events
  glfwSetFramebufferSizeCallback(window, framebufSizeCallback);

  glfwSwapInterval(1);
  return window;
}

void framebufSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

unsigned int compileShaderProgram() {
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Vertex Shader Compilation Failure" << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Fragment Shader Compilation Failure" << infoLog << std::endl;
  }

  // Finally we link both shader objects into a shader program
  unsigned int shaderProgram;
  // glCreateProgram returns an ID ref to newly created program object
  shaderProgram = glCreateProgram();
  // Attach the compiled shaders and link tgr
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

unsigned int loadTextures(const char* path, ASSET_TYPE type) {
  int width, height, nrChannels;
  unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
  unsigned int texture1;
  if (data) {
    // textures are referenced with an ID.
    glGenTextures(1, &texture1);
    // We also need to bind it so subsequent texture commands configures the
    // bound texture
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Start generating texture using loaded image data
    if (type == ASSET_TYPE::PNG)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
    else if (type == ASSET_TYPE::JPG)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture asset" << std::endl;
  }
  stbi_image_free(data);
  return texture1;
}
