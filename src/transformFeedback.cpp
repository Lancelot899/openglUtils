#include "../include/openglUtils/transformFeedback.h"
#include "../include/openglUtils/glTexture.h"


namespace FGLTools {
    FeedbackBuffer::FeedbackBuffer(uint64_t bufferSize_) : bufferSize(bufferSize_) {
        glGenVertexArrays(1,&vao);
        glGenBuffers(1, &vbo);
        glGenTransformFeedbacks(1, &fid);
        char* vertices = new char[bufferSize];
        memset(vertices, 0, bufferSize);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, fid);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_DYNAMIC_DRAW);
        glBindVertexArray(vao);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        delete[] vertices;
        glGenQueries(1, &countQuery);
    }

    FeedbackBuffer::~FeedbackBuffer() {
        glDeleteTransformFeedbacks(1, &fid);
        glDeleteVertexArrays(1,&vao);
        glDeleteBuffers(1, &vbo);
        glDeleteQueries(1, &countQuery);
        CheckGlDieOnError()
    }
    

    void FeedbackBuffer::Bind(GLenum primitiveMode) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, fid);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo);
        glBeginTransformFeedback(primitiveMode);
    }


    void FeedbackBuffer::Unbind() {
        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    }

    void FeedbackBuffer::BindBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);
    }

    void FeedbackBuffer::UnbindBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}