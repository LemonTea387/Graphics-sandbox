#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include <iostream>
#include <cmath>

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 Pos;\n"
    "layout (location = 1) in vec3 InColor;\n"
    "uniform float xOffset;"
    "out vec3 passedColor;\n"
    "void main() {\n"
    "gl_Position = vec4(Pos.x+xOffset, Pos.y, Pos.z, 1.0);\n"
    "passedColor = InColor;\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 passedColor;\n"
    "void main(){\n"
    "FragColor = vec4(passedColor, 1.0);\n"
    "}\0";

GLFWwindow* initGLFWwindow();
unsigned int compileShaderProgram();
void processInput(GLFWwindow* window);
void framebuf_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv) {
  GLFWwindow* window = initGLFWwindow();
  // We also define colors this time
  float vertices[] = {
      -0.7f, -0.7f, 0.0f, 1.0f, 0.0f, 0.0f,  // first
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // second
      -0.5f, 0.2f,  0.0f, 0.0f, 0.0f, 1.0f,  // third

      0.0f,  0.7f,  0.0f, 0.0f, 0.0f, 1.0f,  // first
      0.4f,  0.7f,  0.0f, 0.0f, 1.0f, 0.0f,  // second
      0.8f,  0.2f,  0.0f, 1.0f, 0.0f, 0.0f   // third
  };

  // Prep buffers
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  // For layout 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // layout 1
  // The last offset
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Shaders
  unsigned int shaderProgram = compileShaderProgram();
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 0.5f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    float timeValue = glfwGetTime();
    float xOffset = std::sin(timeValue) * 0.4f;
    int vertexOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
    glUniform1f(vertexOffsetLocation, xOffset);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 2nd one make it 180 out of phase
    xOffset = std::sin(timeValue + 3.14159) * 0.4f;
    glUniform1f(vertexOffsetLocation, xOffset);
    glDrawArrays(GL_TRIANGLES, 3, 3);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  // Delete everything
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  glfwTerminate();
  return 0;
}

GLFWwindow* initGLFWwindow() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sandbox", NULL, NULL);
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
  glViewport(0, 0, 800, 600);
  // Handle resize events
  glfwSetFramebufferSizeCallback(window, framebuf_size_callback);

  return window;
}

void framebuf_size_callback(GLFWwindow* window, int width, int height) {
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
