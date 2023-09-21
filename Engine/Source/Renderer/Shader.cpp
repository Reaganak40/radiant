#include "pch.h"
#include "Shader.h"
#include <fstream>
#include <string>

namespace rdt {

    Shader::Shader()
        : m_ID(0)
    {
    }

    Shader::~Shader()
    {
        if (m_ID) {
            glDeleteProgram(m_ID);
        }
    }

    void Shader::LoadShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
    {
        std::string vertexShader = LoadFromFile(vertexFilepath);
        std::string fragmentShader = LoadFromFile(fragmentFilepath);
        CreateShader(vertexShader, fragmentShader);
    }

    void Shader::LoadDefaultShader()
    {
        std::string vertexShader =
            "#version 450 core\n"
            "\n"
            "layout(location = 0) in vec4 position;\n"
            "layout(location = 1) in vec4 color;\n"
            "layout(location = 2) in vec2 texCoords;\n"
            "layout(location = 3) in float texIndex;\n"
            "\n"
            "smooth out vec4 vColor;\n"
            "out vec2 vTexCoords;\n"
            "out float vTexIndex;\n"
            "\n"
            "uniform mat4 uMVP;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = uMVP * position;\n"
            "    vColor = color;\n"
            "    vTexCoords = texCoords;\n"
            "    vTexIndex = texIndex;\n"
            "};\n";

        std::string fragmentShader =
            "#version 450 core\n"
            "\n"
            "smooth in vec4 vColor;\n"
            "in vec2 vTexCoords;\n"
            "in float vTexIndex;\n"
            "\n"
            "layout(location = 0) out vec4 color;\n"
            "\n"
            "uniform vec4 uColor;\n"
            "uniform sampler2D uTextures[30];\n"
            "\n"
            "void main() {\n"
            "    int index = int(vTexIndex);\n"
            "    color = texture(uTextures[index], vTexCoords) * vColor;\n"
            "}\n";

        CreateShader(vertexShader, fragmentShader);
    }

    int Shader::CompileShader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // TODO: ERROR HANDLING
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

            char* message = new char[length];
            glGetShaderInfoLog(id, length, &length, message);

            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:\n";
            std::cout << message << std::endl;

            delete[] message;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    void Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

        if (m_ID) {
            glDeleteProgram(m_ID);
        }

        m_ID = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(m_ID, vs);
        glAttachShader(m_ID, fs);
        glLinkProgram(m_ID);
        glValidateProgram(m_ID);

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void Shader::Bind(ShaderID program)
    {
        glUseProgram(program);
    }

    UniformID Shader::GetUniform(std::string& variableName) {

        if (m_uniforms.find(variableName) == m_uniforms.end()) {
            UniformID location;
            location = glGetUniformLocation(m_ID, variableName.c_str());

            if (location < 0) {
                std::cout << "Warning: UniformVariable [" << variableName << "] does not exist.\n";
            }

            m_uniforms[variableName] = location;
            return location;
        }

        return m_uniforms[variableName];
    }

    const std::string Shader::LoadFromFile(const std::string& filepath) {
        std::ifstream sFilestream(filepath);

        std::string sFileBuffer;
        if (sFilestream.is_open()) { // always check whether the file is open

            while (sFilestream) {
                std::string line;
                std::getline(sFilestream, line);
                sFileBuffer += line + '\n';
            }
        }
        else {
            std::cout << "Error: Could not open '" << filepath << "'\n";
        }

        sFilestream.close();
        return sFileBuffer;
    }

}