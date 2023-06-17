#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* vertexShaderSource =
    "#version 330 core\n"
    // We handle the 'in' as vec3
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    // at end of main whatever we set gl_Position to will be the output of
    // vertex shader
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    // Frag shaders only requires one output variable of vec4 that defines final
    // color output We just declare output with 'out' keyword, and the name
    // FragColor is just a name we put
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    // Make orange
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void framebuf_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow* initGLFWWindow() {
  // Init glfw and glad
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Sandbox", NULL, NULL);
  // If fail, close
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    std::exit(-1);
  }

  glfwMakeContextCurrent(window);

  // Init GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    std::exit(-1);
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuf_size_callback);
  return window;
}

int main() {
  GLFWwindow* window = initGLFWWindow();
  // A 2D triangle with xyz for each point
  // Uses Normalized Device Coordinates, any outside the range of -1 to 1.0 will
  // be clipped NDC will be transformed to screen space coords via viewport
  // transform
  float vertices[] = {-0.4f, -0.4f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f};

  // Creating memory in GPU to store vertex data, we manage the mem via Vertex
  // buffer objects (VBO) It can store large number of vertices in GPU memory
  // You can send large batches of data all at once to GPU
  // Opengl object, and like other objects, it has unique ID, we generate one
  // with buffer ID using glGenBuffers
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // OpenGL has many types of buffer obj and type of vertex buffer object is
  // GL_ARRAY_BUFFER We can bind several buffers at once as long as they have a
  // diff buffer type
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // From this ponit on any buffer calls on GL_ARRAY_BUFFER target will be used
  // to configure the currently bound buffer, which is VBO.
  // We make a call to glBufferData that copies the vertex data into buffer's
  // mem 4th param specifies how we want GPU to manage given data, either
  // GL_STREAM_DRAW (data set once and used by GPU at most a few times)
  // GL_STATIC_DRAW (data set only once and used many times)
  // GL_DYNAMIC_DRAW (data changed a lot and used many times)
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Now we stored vertex data within memory on graphics card as managed by a
  // Vertex buffer object.
  // Vertex Shader next
  // Opengl has to dynamically compile it at runtime from source code, we need
  // to create shader object
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // attach the shader source code to shader object and compile shader
  // 2nd param is number of strings
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // We can check the status of compilation like this
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Vertex Shader Compilation Failure" << infoLog << std::endl;
  }

  // Fragment shader
  // Calculating the color output of pixels
  // Colors are repr by 4 vals, RGBA, in GLSL we set strength of each component
  // to val between 0.0 -> 1.0
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
  // We can also get the status here
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Shader Program Linking Failure" << infoLog << std::endl;
  }

  // After linking the shader objs we can just delete them
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Since program is linked we can use it now
  // glUseProgram(shaderProgram);

  // We also need to tell opengl how to interpret the vertex data in memory and
  // how to connect vertex data to vertex shader's attribute
  // Vertex shader allows us to specify any input we want in the form of vertex
  // attributes, but this means that we need to manually specify what part of
  // our input data goes into which vertex attribute in vertex shader
  // Since for triangle, our vertex buff data is V1xyz, v2xyz, v3xyz
  // Position data stored as 32-bit floating point vals, each position has 3, no
  // space between each set of 3 vals. First value inthe data is at the
  // beginning of buffer We then tell opengl how to interpret the vertex data
  // per vertex attribute
  //
  // 0 is which vertex attribute we want to configure, in vertex shader we
  // specified the location of position vertex attribute with layout (location =
  // 0) this sets the location of vertex attribute to 0 since we pass data to
  // this vertex attribute, we pass in 0
  //
  // 3 since vertex attribute is vec3
  //
  // a vec* consists of floating point vals
  //
  // GL_False indicates no data normalization, if we input int data types, and
  // we set to true, integer normalised to 0 (or -1 for signed data) and 1 when
  // converted to float
  //
  // 3*sizeof(float) is stride for each consecutive vertex attributes, we could
  // also make stride 0 if array is tightly packed, and let opengl determine
  //
  // last one is offset of where position data begins in buffer, since it starts
  // at beginning of data array we just put 0
  //
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  // (void*)0);
  //
  // Enable the vertex attribute giving the location as argument, they are
  // disabled by default
  //
  // glEnableVertexAttribArray(0);
  //
  // Each vertex attribute takes data from mem managed by VBO, which VBO is
  // deterimed by VBO bound to GL_ARRAY_BUFFER when calling the
  // glVertexAttirbPointer

  // Drawing an object in opengl typically is
  // 0. copy our vertices array in a buffer for OpenGL to use
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 1. then set the vertex attributes pointers
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
  // (void*)0); glEnableVertexAttribArray(0);
  // 2. use our shader program when we want to render an object
  // glUseProgram(shaderProgram);
  // 3. now draw the object
  // someOpenGLFunctionThatDrawsOurTriangle();

  // BUT, WE CAN STORE THE STATE CONFIGURATIONS INTO AN OBJECT THEN JUST BIND
  // THAT LATER This is where we introduce Vertex Array Object (VAO) can be
  // bound just like VBO and subsequent vertex attribute calls from that point
  // will be stored inside VAO.
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // To use VAO just bind it
  // ..:: Initialization code (done once (unless your object frequently
  // changes)) :: ..
  // 1. bind Vertex Array Object
  glBindVertexArray(VAO);
  // 2. copy our vertices array in a buffer for OpenGL to use
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. then set our vertex attributes pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // There is also Element Buffer Objects (EBO), imagine the case of rectangle,
  // we draw 2 triangles, that means 6 vertices, not cool.
  // We can store the unique vertices and then specify the order at which the
  // vertices are drawn. This is indexed drawing
  //
  // float vertices[] = {
  //      0.5f,  0.5f, 0.0f,  // top right
  //      0.5f, -0.5f, 0.0f,  // bottom right
  //     -0.5f, -0.5f, 0.0f,  // bottom left
  //     -0.5f,  0.5f, 0.0f   // top left
  // };
  // unsigned int indices[] = {  // note that we start from 0!
  //     0, 1, 3,   // first triangle
  //     1, 2, 3    // second triangle
  // };
  // unsigned int EBO;
  // glGenBuffers(1, &EBO);
  //
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  // GL_STATIC_DRAW);
  //
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //
  // But this means that we have to bind the corresponding EBO each time to
  // render an obj with indices, so we can use Vertax Array Object again (VAO),
  // the last elem buffer object that gets bound while a vao is bound is stored
  // as the vao's element buffer object, binding to a VAO also automatically
  // binds that EBO

  glUseProgram(shaderProgram);

  // Wireframe mode
  // To draw your triangles in wireframe mode, you can configure how OpenGL
  // draws its primitives via glPolygonMode(GL_FRONT_AND_BACK, GL_LINE). The
  // first argument says we want to apply it to the front and back of all
  // triangles and the second line tells us to draw them as lines. Any
  // subsequent drawing calls will render the triangles in wireframe mode until
  // we set it back to its default using glPolygonMode(GL_FRONT_AND_BACK,
  // GL_FILL).

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.7f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // We draw triangle finally
    // drawarrays draws primitives using current active shader and previously
    // defined vertex attribute configs with VBO 0 is starting index of vertex
    // array 3 is the amount of vertices we want to draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}
