#include "Shader.h"
#include "../GLHead.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, parseShader(vertexPath));
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, parseShader(fragmentPath));

    GLCall(mProgram = glCreateProgram());
    GLCall(glAttachShader(mProgram, vs));
    GLCall(glAttachShader(mProgram, fs));
    GLCall(glLinkProgram(mProgram));
    GLCall(glValidateProgram(mProgram));

    int result;
    GLCall(glGetProgramiv(mProgram, GL_LINK_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)alloca(length * sizeof(char *));
        GLCall(glGetProgramInfoLog(mProgram, length, &length, message));
        std::cout << "Failed to Link " << message << std::endl;
    }
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(mProgram));
}

void Shader::begin()
{
    GLCall(glUseProgram(mProgram));
}
void Shader::end()
{
    GLCall(glUseProgram(0));
}

// vs 与 fs 的shader 的Uniform 重名的时候，都会起作用
void Shader::setUniformValue(const std::string& name, float value) 
{
    // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));

    // 通过location来更新Uniform变量
     GLCall(glUniform1f(locaiton, value));
}

void Shader::setUniformValue(const std::string& name, float* value) 
{
    // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));

    // 通过location来更新Uniform变量
    GLCall(glUniform3fv(locaiton,  1, value));
}

void Shader::setUniformValue(const std::string& name, const glm::vec3& value)
{
        // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));

    // 通过location来更新Uniform变量
    GLCall(glUniform3f(locaiton,  value.x, value.y, value.z));
}

void Shader::setUniformValue(const std::string& name, int value) {
        // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));

    // 通过location来更新Uniform变量
    GLCall(glUniform1i(locaiton, value));
}

void Shader::setUnifromValue(const std::string& name, const glm::mat4& value) {
     // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));
    // opengl 和 glm 矩阵的存储方式都是列存储，所以不需要转置
    GLCall(glUniformMatrix4fv(locaiton, 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::setUnifromValue(const std::string& name, const glm::mat3& value) {
     // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));
    // opengl 和 glm 矩阵的存储方式都是列存储，所以不需要转置
    GLCall(glUniformMatrix3fv(locaiton, 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::setUnifromValue(const std::string& name, const glm::mat4* value, int count) {
    // 通过名称拿到Uniform的位置
    GLCall(GLuint locaiton = glGetUniformLocation(mProgram, name.c_str()));
    // opengl 和 glm 矩阵的存储方式都是列存储，所以不需要转置
    GLCall(glUniformMatrix4fv(locaiton, count, GL_FALSE, glm::value_ptr(value[0])));
}

std::string Shader::parseShader(const std::string& filepath)
{
   std::ifstream stream(filepath);
   std::string line;
   std::stringstream ss;
   while (getline(stream, line)) {
       ss << line << '\n';
   }
   std::cout << "Shader File Content : " << ss.str() << std::endl;

   return ss.str();

    // std::string shaderCode = "";
    // std::ifstream shaderFile;

    // shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // try
    // {
    //     shaderFile.open(filePath);

    //     std::stringstream shaderStream;
    //     shaderStream << shaderFile.rdbuf();


    //     shaderFile.close();

    //     shaderCode = shaderStream.str();
    // }
    // catch (std::ifstream::failure &e)
    // {
    //     std::cout << "ERROR : Shader File Error : " << e.what() << std::endl;
    // }

    // std::cout << "Shader File Content : " << shaderCode << std::endl;

    // return shaderCode.c_str();
}

GLuint Shader::compileShader(unsigned int type, const std::string& source)
{
    // 1、创建shader程序
    GLCall(GLuint id = glCreateShader(type));

    // 2、为shader程序输入shader代码
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));

    // 3、编译shader代码
    GLCall(glCompileShader(id));

    // 4、查看编译结果
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *message = (char *)alloca(length * sizeof(char *));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? " vertexShader" : "fragmentShader") << " shader !" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}