#pragma once
#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>


class Shader
{
public:
    Shader(const std::string& filepath);
    unsigned int GetProgramID();
    ~Shader();


private:
    unsigned int program_id;
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
        std::string NotRecognized;
    };
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};