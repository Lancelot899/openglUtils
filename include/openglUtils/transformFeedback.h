#pragma once

#include "glPlatform.h"
#include <memory>

namespace FGLTools {

class FeedbackBuffer {
public:
    FeedbackBuffer(uint64_t bufferSize);
    virtual ~FeedbackBuffer();
    const uint64_t bufferSize;
    GLuint vbo;
    GLuint vao;
    GLuint fid;
    GLuint countQuery;
    GLuint count = 0;
    void Bind(GLenum primitiveMode);
    void Unbind();
    void BindBuffer();
    void UnbindBuffer();
};

}