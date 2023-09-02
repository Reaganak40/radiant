#pragma once
#include "pch.h"

namespace Radiant {

#define ASSERT(x) if (!(x)) __debugbreak();

#define glCall(x) glClearError(); x; ASSERT(glLogError(#x, __FILE__, __LINE__))

    void glClearError();
    bool glLogError(const char* function, const char* filename, int line);
    std::string glErrorContext(unsigned int error);

    void GLAPIENTRY
        MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam);

    void EnableErrorCallback();
}