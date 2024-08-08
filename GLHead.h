#ifndef GLHEAD_H
#define GLHEAD_H

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __builtin_debugtrap();

#ifdef DEBUG
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x;
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#endif // GLHEAD_H