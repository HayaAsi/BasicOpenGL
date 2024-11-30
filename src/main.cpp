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
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "./assignment1.h"
#include <iostream>

/* Window size */
const unsigned int width = 800;
const unsigned int height = 800;
// const float FOVdegree = 45.0f;  // Field Of View Angle
const float near = 0.1f;
const float far = 100.0f;

/* Shape vertices coordinates with positions, colors, and corrected texCoords */
float vertices[] = {
    // positions            // colors            // texCoords
    -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  // Top-right
    -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,  // Top-left
};

/* Indices for vertices order */
unsigned int indices[] = {
    0, 1, 2, 
    2, 3, 0
};

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    
    /* Set OpenGL to Version 3.3.0 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Load GLAD so it configures OpenGL */
    gladLoadGL();

    /* Control frame rate */
    glfwSwapInterval(1);

    /* Print OpenGL version after completing initialization */
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    /* Set scope so that on widow close the destructors will be called automatically */
    {
        /* Blend to fix images with transperancy */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        /* Generate VAO, VBO, EBO and bind them */
        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indices, sizeof(indices));

        VertexBufferLayout layout;
        layout.Push<float>(3);  // positions
        layout.Push<float>(3);  // colors
        layout.Push<float>(2);  // texCoords
        va.AddBuffer(vb, layout);
        std::string filepath = "res/textures/Lenna.png";
        int width, height, comps;
        int req_comps = 4;
        unsigned char * buffer = stbi_load(filepath.c_str(), &width, &height, &comps, req_comps);
        stbi_write_png("res/textures/gray.png", width, height, req_comps, convertToGrayscale(buffer, width, height), width * comps);
        stbi_write_png("res/textures/edge.png", width, height, req_comps, Canny_Edge_Detector(buffer, width, height), width * comps);
        stbi_write_png("res/textures/halftone.png", width*2, height*2, req_comps, convertToHalftone(buffer, width, height), width * comps);
        stbi_write_png("res/textures/floyd.png", width, height, req_comps, convertToFloyd(buffer, width, height), width * comps);
        unsigned char * data = convertToHalftone(buffer, width, height);
        int counter = 0;
        FILE* f = fopen("../halftone.txt", "w");
        for(int i = 0;i<width*2;i++){
            for(int j = 0;j<width*2;j++){
                fprintf(f, "%x, ", data[counter]);
                counter+=4;
            }   
            fprintf(f, "%\n");
        }
        /* Create texture */
        Texture texture1("res/textures/gray.png");
        Texture texture2("res/textures/edge.png");
        Texture texture3("res/textures/halftone.png");
        Texture texture4("res/textures/floyd.png");
         
        /* Create shaders */
        Shader shader("res/shaders/real.shader");
        shader.Bind();

        /* Unbind all to prevent accidentally modifying them */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        /* Enables the Depth Buffer */
    	GLCall(glEnable(GL_DEPTH_TEST));

        /* Create camera */
        Camera camera(width, height);
        camera.SetOrthographic(near, far);
        camera.EnableInputs(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Set white background color */
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            /* Initialize uniform color */
            glm::vec4 color = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);
            /* Initialize view and projection matrices */
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 proj = camera.GetProjectionMatrix();

            /* Render first texture (top-left) */
            texture1.Bind();
            glm::mat4 trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, -1.0f));
            glm::mat4 scl1 = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
            glm::mat4 mvp1 = proj * view * (trans1 * scl1);
            shader.Bind();
            shader.SetUniform4f("u_Color", color);
            shader.SetUniformMat4f("u_MVP", mvp1);
            shader.SetUniform1i("u_Texture", 0);
            va.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

            /* Render second texture (top-right) */
            texture2.Bind();
            glm::mat4 trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, -1.0f));
            glm::mat4 scl2 = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
            glm::mat4 mvp2 = proj * view * (trans2 * scl2);
            shader.SetUniformMat4f("u_MVP", mvp2);
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

            /* Render third texture (bottom-left) */
            texture3.Bind();
            glm::mat4 trans3 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -1.0f));
            glm::mat4 scl3 = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
            glm::mat4 mvp3 = proj * view * (trans3 * scl3);
            shader.SetUniformMat4f("u_MVP", mvp3);
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

            /* Render fourth texture (bottom-right) */
            texture4.Bind();
            glm::mat4 trans4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.5f, -1.0f));
            glm::mat4 scl4 = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
            glm::mat4 mvp4 = proj * view * (trans4 * scl4);
            shader.SetUniformMat4f("u_MVP", mvp4);
            GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}