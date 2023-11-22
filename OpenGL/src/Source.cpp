#define _CRT_SECURE_NO_WARNINGS
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


float rotation = 0;

unsigned int load_BMP(const char* path)
{
    // BMP header
    unsigned char header[54];

    // Position of data in the file
    unsigned int dataPos;

    // Width and height of the image
    unsigned int width, height;
    unsigned int imageSize;

    // Data container
    unsigned char* data;

    FILE* file = fopen(path, "rb");
    if (!file)
    {
        std::cout << "File cannot be open.\n";
        return -1;
    }

    // Read the header
    if (fread(header, 1, 54, file) != 54)
    {
        printf("Wrong file format\n");
        return -1;
    }

    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Wrong file format\n");
        return -1;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (imageSize == 0)
        imageSize = width * height * 3;
    
    if (dataPos == 0)
        dataPos = 54;

    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_X && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        rotation -= 0.1f;
    }

    if (key == GLFW_KEY_Z && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        rotation += 0.1f;
    }
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
    std::string NotRecognized;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = 2, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
  
    // For every line
    while (getline(stream, line, '\n'))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return {ss[0].str(), ss[1].str(), ss[2].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // Failed compilation
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile shader: " << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    // Pointer to GLFW struct
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;

    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 900;

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
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    ShaderProgramSource source = ParseShader("shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
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

    GLuint MatrixID = glGetUniformLocation(shader, "MVP");

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    const float radius = 2.75f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;

    GLuint Texture = load_BMP("textures/text.bmp");

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

        glfwSetKeyCallback(window, key_callback);
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}