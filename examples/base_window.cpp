#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/**
 * Callback function on window that gets called each time the window is resized.
 * We want to resize the viewport for opengl.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  // If not pressed it return GLFW_RELEASE
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  std::cout << "Hello World Test!" << std::endl;

  // Instantiate GLFW Window
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFW window object
  // Holds all windowing data and is required by most GLFW other functions
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL SandBox", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Makes the context of specific window current for the calling thread
  glfwMakeContextCurrent(window);

  // Initialise GLAD before any opengl functions as GLAD manages function
  // pointers for Opengl We provied the function to load addr of opengl function
  // pointers that are os-specific, for GLFW, it is glfwGetProcAddress that
  // defines the correct function based on which OS we compiling for
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Tell opengl the size of the rendering window so opengl knows how we want to
  // display data and coords wrt to window
  // lower left, corner for first 2, width,height in pixels
  glViewport(0, 0, 800, 600);

  // Set callback for viewport size change
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Render loop
  // Checks at start of each loop if GLFW has been instructed to close
  while (!glfwWindowShouldClose(window)) {
    // Clear the screen or will see results of previous frame
    // This is state setting function
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // there's also DEPTH_BUFFER_BIT, STENCIL_BUFFER_BIT, but currently we only
    // care about color This is a state-using function that uses the current
    // state to retrieve the clearing color
    glClear(GL_COLOR_BUFFER_BIT);
    // Input
    processInput(window);
    // Swap the color buffer(a large 2D buffer that contains color values for
    // each pixel in GLFW window) that is used to render during this render
    // iteration Double buffers, front to display, back to render to avoid
    // flickering isues
    glfwSwapBuffers(window);
    // Check for any events like keyboard input/mousemovement and updates window
    // state, and calls corresponding functions(we register them through
    // callback functions like the size change above)
    glfwPollEvents();
  }

  // Properly clean/delete all GLFW's resources
  glfwTerminate();
  return 0;
}
