//
// Created by 范帝楷 on 2020/5/29.
//

#include "../include/openglUtils/glTexture.h"

namespace FGLTools {


GlRenderBuffer::GlRenderBuffer()
    : width(0), height(0), rbid(0)
{
}

GlRenderBuffer::GlRenderBuffer(GLint width, GLint height, GLint internal_format )
    : width(0), height(0), rbid(0) {
  Reinitialise(width,height,internal_format);
}

#ifndef HAVE_GLES

void GlRenderBuffer::Reinitialise(GLint width, GLint height, GLint internal_format) {
  if( this->width != 0 ) {
    glDeleteRenderbuffers(1, &rbid);
  }

  this->width = width;
  this->height = height;
  glGenRenderbuffers(1, &rbid);
  glBindRenderbuffer(GL_RENDERBUFFER, rbid);
  glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GlRenderBuffer::~GlRenderBuffer() {
  if( width!=0 ) {
    glDeleteRenderbuffers(1, &rbid);
  }
}
#else

void GlRenderBuffer::Reinitialise(GLint width, GLint height, GLint internal_format) {
    if( width!=0 ) {
        glDeleteTextures(1, &rbid);
    }

    // Use a texture instead...
    glGenTextures(1, &rbid);
    glBindTexture(GL_TEXTURE_2D, rbid);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
            width, height,
            0, internal_format, GL_UNSIGNED_SHORT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

GlRenderBuffer::~GlRenderBuffer() {
    if( width!=0 && !pangolin::ShouldQuit() ) {
        glDeleteTextures(1, &rbid);
    }
}
#endif // HAVE_GLES

GlRenderBuffer::GlRenderBuffer(GlRenderBuffer&& tex)
    : width(tex.width), height(tex.height), rbid(tex.rbid) {
  tex.rbid = tex.width = tex.height = 0;
}



}