//
// Created by 范帝楷 on 2020/5/29.
//

#include "../include/openglUtils/glTexture.h"


namespace FGLTools {

#ifndef HAVE_GLES
const int MAX_ATTACHMENTS = 8;
const static GLuint attachment_buffers[] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7
};
#else // HAVE_GLES
const int MAX_ATTACHMENTS = 1;
const static GLuint attachment_buffers[] = {
    GL_COLOR_ATTACHMENT0_EXT
};
#endif // HAVE_GLES


GlFramebuffer::GlFramebuffer()
    : fbid(0), attachments(0)
{
}

GlFramebuffer::~GlFramebuffer()
{
  if(fbid) {
    glDeleteFramebuffersEXT(1, &fbid);
  }
}

GlFramebuffer::GlFramebuffer(GlTexture& colour, GlRenderBuffer& depth)
    : attachments(0) {
  glGenFramebuffers(1, &fbid);
  AttachColour(colour);
  AttachDepth(depth);
  CheckGlDieOnError();
}

GlFramebuffer::GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlRenderBuffer& depth)
    : attachments(0) {
  glGenFramebuffers(1, &fbid);
  AttachColour(colour0);
  AttachColour(colour1);
  AttachDepth(depth);
  CheckGlDieOnError();
}

GlFramebuffer::GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlTexture& colour2, GlRenderBuffer& depth)
    : attachments(0) {
  glGenFramebuffers(1, &fbid);
  AttachColour(colour0);
  AttachColour(colour1);
  AttachColour(colour2);
  AttachDepth(depth);
  CheckGlDieOnError();
}

GlFramebuffer::GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlTexture& colour2, GlTexture& colour3, GlRenderBuffer& depth)
    : attachments(0) {
  glGenFramebuffers(1, &fbid);
  AttachColour(colour0);
  AttachColour(colour1);
  AttachColour(colour2);
  AttachColour(colour3);
  AttachDepth(depth);
  CheckGlDieOnError();
}

void GlFramebuffer::Bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbid);
#ifndef HAVE_GLES
  glDrawBuffers( attachments, attachment_buffers );
#endif
}

void GlFramebuffer::Reinitialise() {
  if(fbid) {
    glDeleteFramebuffers(1, &fbid);
  }
  glGenFramebuffers(1, &fbid);
}

void GlFramebuffer::Unbind() const {
#ifndef HAVE_GLES
  glDrawBuffers( 1, attachment_buffers );
#endif
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLenum GlFramebuffer::AttachColour(GlTexture& tex ) {
  if(!fbid) Reinitialise();

  const GLenum color_attachment = GL_COLOR_ATTACHMENT0 + attachments;
  glBindFramebuffer(GL_FRAMEBUFFER, fbid);
  glFramebufferTexture2D(GL_FRAMEBUFFER, color_attachment, GL_TEXTURE_2D, tex.tid, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  attachments++;
  CheckGlDieOnError();
  return color_attachment;
}

void GlFramebuffer::AttachDepth(GlRenderBuffer& rb ) {
  if(!fbid) Reinitialise();

  glBindFramebuffer(GL_FRAMEBUFFER, fbid);
#if !defined(HAVE_GLES)
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.rbid);
#elif defined(HAVE_GLES_2)
  glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, rb.rbid, 0);
#else
    throw std::exception();
#endif
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CheckGlDieOnError();
}

}