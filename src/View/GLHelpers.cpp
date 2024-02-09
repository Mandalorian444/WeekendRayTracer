#include "GLHelpers.h"

#include <gl/glew.h>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    { // assignment will return 0 if glGetError returns 0, so this works
        std::cout << "[OpenGL Error] (0x" << std::hex << error << std::dec
                  << "): " << function << " " << file << ":" << line
                  << std::endl;
        return false;
    }
    return true;
}

void CallGlewInit()
{
    if (glewInit() != GLEW_OK)
    {
        printf("GLEW was unable to init");
        std::terminate();
    }
}
