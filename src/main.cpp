#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <RubikCube.h>
#include <iostream>

const unsigned int width = 800;
const unsigned int height = 800;
const float FOVdegree = 45.0f;
const float near = 0.1f;
const float far = 100.0f;

float vertices[] = {
    // positions           // colors           // texCoords
    // Front face (Red)
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom-left (Red)
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Bottom-right (Red)
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top-right (Red)
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, // Top-left (Red)

    // Back face (Orange)
    -0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left (Orange)
     0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 0.0f, // Bottom-right (Orange)
     0.5f,  0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 1.0f, // Top-right (Orange)
    -0.5f,  0.5f, -0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 1.0f, // Top-left (Orange)

    // Left face (Green)
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom-left (Green)
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right (Green)
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top-right (Green)
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Top-left (Green)

    // Right face (Blue)
     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom-left (Blue)
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom-right (Blue)
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-right (Blue)
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Top-left (Blue)

    // Top face (White)
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom-left (White)
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right (White)
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right (White)
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left (White)

    // Bottom face (Yellow)
    -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom-left (Yellow)
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right (Yellow)
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top-right (Yellow)
    -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top-left (Yellow)
};

unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

int main(int argc, char* argv[]) {
    GLFWwindow* window;
    RubikCube cube;
    
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Rubik Cube", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    {
        /* Enable depth testing */
        GLCall(glEnable(GL_DEPTH_TEST));
    
        /* Set up buffers and shaders */
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indices, sizeof(indices));
        
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        Texture texture("res/textures/plane.png");
        texture.Bind();

        Shader shader("res/shaders/basic.shader");
        shader.Bind();

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();
        cube.shader = &shader;
        cube.vertexArray = &va;
        cube.vertexBuffer = &vb;
        cube.indexBuffer = &ib;
        cube.window = window;

        /* Initialize the camera */
        Camera camera(width, height, cube);
        camera.SetPerspective(FOVdegree, near, far);
        camera.TranslateView(glm::vec3(0.0f, 0.0f, -10.0f));
        camera.EnableInputs(window);
        while (!glfwWindowShouldClose(window)) {
            cube.setProj(camera.GetProjectionMatrix());
            cube.setView(camera.GetViewMatrix());
            cube.render(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}