#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string.h>

#include <iostream>
#include "stb_image.h"

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 Pos;\n"
    "layout (location = 1) in vec3 InColor;\n"
    "layout (location = 2) in vec2 TexCoord;\n"
    "uniform float xOffset;"
    "out vec3 passedColor;\n"
    "out vec2 passedTexCoord;\n"
    "void main() {\n"
    "gl_Position = vec4(Pos.x+xOffset, Pos.y, Pos.z, 1.0);\n"
    "passedColor = InColor;\n"
    "passedTexCoord = TexCoord;\n"
    "}\0";
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 passedColor;\n"
    "in vec2 passedTexCoord;\n"
    "uniform float xOffset;\n"
    // GLSL built-in type for texture objects called sampler
    // We can actually assign this elsewhere with glUniform1i, with a location
    // value, or texture unit, by default it is 0, which is the default active
    // texture unit.
    "uniform sampler2D Texture1;\n"
    "uniform sampler2D Texture2;\n"
    "void main(){\n"
    "FragColor = mix(texture(Texture1, passedTexCoord), texture(Texture2, passedTexCoord), 0.2) * vec4(passedColor+xOffset,1.0);\n"
    "}\0";

unsigned char* stbi_load(const char* source, int* width, int* height,
                         int* nrChannels, int);
void stbi_image_free(void* retval_from_stbi_load);
GLFWwindow* initGLFWwindow();
unsigned int compileShaderProgram();
void processInput(GLFWwindow* window);
void framebuf_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv) {
  GLFWwindow* window = initGLFWwindow();
  // We also define colors this time
  float vertices[] = {
      // positions        // colors         // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // layout 1
  // The last offset
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // layout 2
  // Texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Shaders
  unsigned int shaderProgram = compileShaderProgram();

  // Load textures
  // Images usually have 0.0 at top of y-axis, but OpenGL expects 0.0 at bottom
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data =
      stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
  unsigned int texture1;
  if (data) {
    // textures are referenced with an ID.
    glGenTextures(1, &texture1);
    // We also need to bind it so subsequent texture commands configures the
    // bound texture
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // set texture wrapping to GL_REPEAT (default
                                 // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Start generating texture using loaded image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    std::cout << "Texture loaded" << std::endl;
  } else {
    std::cout << "Failed to load texture asset" << std::endl;
  }
  // Done with gen texture and mipmap, free the img mem
  stbi_image_free(data);

  data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
  unsigned int texture2;
  if (data) {
    // textures are referenced with an ID.
    glGenTextures(1, &texture2);
    // Start generating texture using loaded image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glGenerateMipmap(GL_TEXTURE_2D);
    std::cout << "Texture loaded" << std::endl;
  } else {
    std::cout << "Failed to load texture asset" << std::endl;
  }
  // Done with gen texture and mipmap, free the img mem
  stbi_image_free(data);

  float timeValue;
  float xOffset;
  int vertexOffsetLocation{glGetUniformLocation(shaderProgram, "xOffset")};
  // Binding textures to corresponding texture unit
  glActiveTexture(
      GL_TEXTURE0);  // activate texture unit first before binding texture
  // Subsequent texture binds will bind to this texture unit
  // OpenGL guarantees a min of 16 texture units
  glBindTexture(GL_TEXTURE_2D, texture1);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glUniform1i(glGetUniformLocation(shaderProgram, "Texture1"), 0);
  glUniform1i(glGetUniformLocation(shaderProgram, "Texture2"), 1);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glClearColor(0.0f, 0.5f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    timeValue = glfwGetTime();
    xOffset = sin(timeValue) * 0.4f;
    glUniform1f(vertexOffsetLocation, xOffset);

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

  glfwSwapInterval(1);
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
