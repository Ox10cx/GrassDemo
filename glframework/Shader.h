#ifndef SHADER_H
#define SHADER_H
#include "core.h"
#include <string>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void begin();
    void end();
    void setUniformValue(const std::string& name, float value);
    void setUniformValue(const std::string& name, float* value);
    void setUniformValue(const std::string& name, const glm::vec3& value);
    void setUniformValue(const std::string& name, int value);
    void setUnifromValue(const std::string& name, const glm::mat4& value);
    void setUnifromValue(const std::string& name, const glm::mat4* value, int count);
    void setUnifromValue(const std::string& name, const glm::mat3& value);

private:
    GLuint compileShader(unsigned int type, const std::string& source);
    std::string parseShader(const std::string& filepath);

private:
    GLuint mProgram{0};
};

#endif