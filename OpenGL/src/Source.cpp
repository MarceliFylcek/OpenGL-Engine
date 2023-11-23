#include <gl/glew.h> //always first
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include "utils.h"
#include "controls.h"
#include "shader.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 900


float InputController::rotation = 0.0f;

void updateProjectionMatrix()
{

}

int main(void)
{
    // Pointer to GLFW struct
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error" << std::endl;
        return -1;
    }

    float positions[] =
    {
    // Left
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,

    -1.0f,  1.0f,  1.0f, 
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    // Right
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,

     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,

    // Front
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,

     1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
 
    // Back
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,

     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    // Top
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,

     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    // Bottom
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,

     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,

    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    unsigned int row = 3;
    unsigned int column = 0;
    static const GLfloat g_uv_buffer_data[] = {
        // Texture coordinates for the front face
    (0.0f + float(column)) / 7.0f,   (0.0f + row) / 7.0f,
    (1.0f + float(column)) / 7.0f,   (0.0f + row) / 7.0f,
    (1.0f + float(column)) / 7.0f,   (1.0f + row) / 7.0f,

    (1.0f + float(column)) / 7.0f,   (1.0f + row) / 7.0f,
    (0.0f + float(column)) / 7.0f,   (0.0f + row) / 7.0f,
    (0.0f + float(column)) / 7.0f,   (1.0f + row) / 7.0f,

    (0.0f + float(column+1)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+1)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+1)) / 7.0f, (1.0f + row) / 7.0f,

    (1.0f + float(column+1)) / 7.0f, (1.0f + row) / 7.0f,
    (0.0f + float(column+1)) / 7.0f, (0.0f + row) / 7.0f,
    (0.0f + float(column+1)) / 7.0f, (1.0f + row) / 7.0f,

    (0.0f + float(column+2)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+2)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+2)) / 7.0f, (1.0f + row) / 7.0f,

    (1.0f + float(column+2)) / 7.0f, (1.0f + row) / 7.0f,
    (0.0f + float(column+2)) / 7.0f, (0.0f + row) / 7.0f,
    (0.0f + float(column+2)) / 7.0f, (1.0f + row) / 7.0f,

    (0.0f + float(column+3)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+3)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+3)) / 7.0f, (1.0f + row) / 7.0f,

    (1.0f + float(column+3)) / 7.0f, (1.0f + row) / 7.0f,
    (0.0f + float(column+3)) / 7.0f, (0.0f + row) / 7.0f,
    (0.0f + float(column+3)) / 7.0f, (1.0f + row) / 7.0f,

    (0.0f + float(column+4)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+4)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+4)) / 7.0f, (1.0f + row) / 7.0f,

    (1.0f + float(column+4)) / 7.0f, (1.0f + row) / 7.0f,
    (0.0f + float(column+4)) / 7.0f, (0.0f + row) / 7.0f,
    (0.0f + float(column+4)) / 7.0f, (1.0f + row) / 7.0f,

    (0.0f + float(column+5)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+5)) / 7.0f, (0.0f + row) / 7.0f,
    (1.0f + float(column+5)) / 7.0f, (1.0f + row) / 7.0f,

    (1.0f + float(column+5)) / 7.0f, (1.0f + row) / 7.0f,
    (0.0f + float(column+5)) / 7.0f, (0.0f + row) / 7.0f,
    (0.0f + float(column+5)) / 7.0f, (1.0f + row) / 7.0f,
    };

    unsigned int texturebuffer;
    glGenBuffers(1, &texturebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                // attribute.
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         //
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Shader *shader = new Shader("shaders/Basic.shader");
    unsigned int program_id = (*shader).GetProgramID();

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    std::cout << glGetString(GL_VERSION) << std::endl;
    static float red = 0.0f;

    glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 640.0f / 480.0f, 0.1f, 100.0f);

    glm::mat4 View = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 mvp = Projection * View * Model;

    GLuint MatrixID = glGetUniformLocation(program_id, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    const float radius = 2.75f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    GLuint Texture = load_BMP("textures/text.bmp");
    InputController& inputController = InputController::get_instance();

    glfwSetKeyCallback(window, inputController.key_callback);
    glfwSetCursorPosCallback(window, inputController.cursor_position_callback);

    float rotation = inputController.get_rotation();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        camX = sin(rotation) * radius;
        camZ = cos(rotation) * radius;
        View = glm::lookAt(
            glm::vec3(camX, 1.5, camZ),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

        mvp = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glClearColor(4.0f, 0.0f, 0.4f, 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        std::this_thread::sleep_for(std::chrono::milliseconds(15));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        rotation = inputController.get_rotation();

    }

    delete shader;
    glfwTerminate();
    return 0;
}