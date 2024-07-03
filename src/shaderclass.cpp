// Shader abstraction
// Camera abstraction
// Rewrite code for main.cpp

// Read shader and return c_str()
#include "shaderclass.hpp"

void CheckCompileErrors(GLuint shader)
{
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR: "
                  << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
void CheckLinkingErrors(GLuint prog)
{
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(prog, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR: "
                  << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
GLuint ReadCompileShader(const std::string &pathToShader, GLenum typeOfShader)
{
    std::ifstream shaderFile(pathToShader);
    if (shaderFile.fail())
    {
        std::cout << "Failed to read " << pathToShader << "...\n";
        return 0;
    }

    std::ostringstream contents;
    contents << shaderFile.rdbuf();
    shaderFile.close();

    const std::string tempShaderCode = contents.str();
    const char *shaderCode = tempShaderCode.c_str();

    GLuint shader = glCreateShader(typeOfShader);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    CheckCompileErrors(shader);

    return shader;
}

Shader::Shader(const std::string &pathToVertexShader, const std::string &pathToFragmentShader)
{
    GLuint vertex, fragment;
    vertex = ReadCompileShader(pathToVertexShader, GL_VERTEX_SHADER);
    fragment = ReadCompileShader(pathToFragmentShader, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckLinkingErrors(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
Shader::~Shader()
{
    glDeleteProgram(ID);
}
void Shader::use() const
{
    glUseProgram(ID);
}
void Shader::stop() const
{
    glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
