//
// Created by 范帝楷 on 2020/5/29.
//
#include "../include/openglUtils/glPlatform.h"
#include <stdio.h>
#include "../include/openglUtils/glPixFormat.h"
#include "../include/openglUtils/glTexture.h"

namespace FGLTools {
void _CheckGlDieOnError( const char *sFile, const int nLine )
{
  const GLenum glError = glGetError();
  if( glError != GL_NO_ERROR ) {
    printf("OpenGL Error %x\n", glError);
    printf("In: %s, line %d\n", sFile, nLine);
  }
}
//glformat：
//GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER,
// GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.

// type
//GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2,
// GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4,
// GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8,
// GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV.
GlTexture::GlTexture(
    GLint width,
    GLint height,
    GLint internal_format,
    bool sampling_linear,
    int border,
    GLenum glformat,
    GLenum gltype,
    GLvoid* data)
    : internal_format(0), tid(0) {
    
  Reinitialise(width, height, internal_format, sampling_linear, border, glformat, gltype, data);
        
}



 GlTexture::GlTexture(const cv::Mat& img,bool Need_Normal, bool sampling_linear)
     : internal_format(0), tid(0) {
         this->Load(img, Need_Normal,sampling_linear);
    }

GlTexture::GlTexture(const cv::Mat& img, bool sampling_linear)
  : internal_format(0), tid(0) {
  this->Load(img, sampling_linear);
}

GlTexture::GlTexture(GlTexture&& tex) {
  *this = std::move(tex);
}

GlTexture& GlTexture::operator=(GlTexture&& tex) {
  if (&tex != this) {
    internal_format = tex.internal_format;
    tid = tex.tid;
    width = tex.width;
    height = tex.height;

    tex.internal_format = 0;
    tex.tid = 0;
  }
  return *this;
}

bool GlTexture::IsValid() const {
  return tid != 0;
}

void GlTexture::Delete() {
  // We have no GL context whilst exiting.
  if (internal_format != 0) {
    glDeleteTextures(1, &tid);
    internal_format = 0;
    tid = 0;
    width = 0;
    height = 0;
  }
}

GlTexture::~GlTexture() {
  // We have no GL context whilst exiting.
  if (internal_format != 0) {
    glDeleteTextures(1, &tid);
  }
}

void GlTexture::Bind() const {
  glBindTexture(GL_TEXTURE_2D, tid);
}

void GlTexture::Unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void GlTexture::Reinitialise(
    GLsizei w,
    GLsizei h,
    GLint int_format,
    bool sampling_linear,
    int border,
    GLenum glformat,
    GLenum gltype,
    GLvoid* data) {
    CheckGlDieOnError();
    if (tid != 0) {
      glDeleteTextures(1, &tid);
    }
    CheckGlDieOnError();
    
    internal_format = int_format;
    width = w;
    height = h;
    glGenTextures(1, &tid);
    
    CheckGlDieOnError();
    Bind();
    
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, border, glformat, gltype, data);
    
    CheckGlDieOnError();

    if (sampling_linear) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    CheckGlDieOnError();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CheckGlDieOnError();
    Unbind();
}

// format
//GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_DEPTH_COMPONENT, and GL_STENCIL_INDEX.
//type
//GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2,
// GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
// GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2,
// and GL_UNSIGNED_INT_2_10_10_10_REV.
void GlTexture::Upload(const void* data, GLenum data_format, GLenum data_type) {
    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, data_format, data_type, data);
    CheckGlDieOnError();
    Unbind();
}

void GlTexture::Upload(
    const void* data,
    GLsizei tex_x_offset,
    GLsizei tex_y_offset,
    GLsizei data_w,
    GLsizei data_h,
    GLenum data_format,
    GLenum data_type) {
    Bind();
    glTexSubImage2D(
          GL_TEXTURE_2D, 0, tex_x_offset, tex_y_offset, data_w, data_h, data_format, data_type, data);
    CheckGlDieOnError();
    Unbind();
}

void GlTexture::Load(const cv::Mat& image, bool sampling_linear) {
  GlPixFormat fmt(image);
  Reinitialise(
      (GLint)image.cols,
      (GLint)image.rows,
      fmt.scalable_internal_format,
      sampling_linear,
      0,
      fmt.glformat,
      fmt.gltype,
      image.data);
}

void GlTexture::Load(const cv::Mat& img, bool Need_Normal,bool sampling_linear){
    GlPixFormat fmt(img,Need_Normal);
    Reinitialise(
            (GLint)img.cols,
            (GLint)img.rows,
            fmt.scalable_internal_format,
            sampling_linear,
            0,
            fmt.glformat,
            fmt.gltype,
            img.data);
}


// format
//
//    Specifies a pixel format for the returned data. The supported formats are GL_STENCIL_INDEX, GL_DEPTH_COMPONENT,
//    GL_DEPTH_STENCIL, GL_RED, GL_GREEN, GL_BLUE, GL_RG, GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RED_INTEGER, GL_GREEN_INTEGER,
//    GL_BLUE_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER, GL_BGR_INTEGER, GL_BGRA_INTEGER.

// type
//
//    Specifies a pixel type for the returned data. The supported types are GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT,
//    GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV,
//    GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV,
//    GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV,
//    GL_UNSIGNED_INT_10_10_10_2, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_24_8, GL_UNSIGNED_INT_10F_11F_11F_REV,
//    GL_UNSIGNED_INT_5_9_9_9_REV, and GL_FLOAT_32_UNSIGNED_INT_24_8_REV.
#ifndef HAVE_GLES
void GlTexture::Download(void* image, GLenum format, GLenum data_type) const {
  Bind();
  glGetTexImage(GL_TEXTURE_2D, 0, format, data_type, image);
  Unbind();
}

void GlTexture::Download(cv::Mat& image) const {
  switch (internal_format) {
      case GL_RED:
          image = cv::Mat::zeros(height, width, CV_8UC1);
          Download(image.data, GL_RED, GL_UNSIGNED_BYTE);
          break;
      case GL_R8:
          image = cv::Mat::zeros(height, width, CV_8UC1);
          Download(image.data, GL_RED, GL_UNSIGNED_BYTE);
          break;
      case GL_RED_INTEGER:
          image = cv::Mat::zeros(height, width, CV_16UC1);
          Download(image.data, GL_RED_INTEGER, GL_UNSIGNED_SHORT);
          break;
      case GL_R16UI:
          image = cv::Mat::zeros(height, width, CV_16UC1);
          Download(image.data, GL_RED_INTEGER, GL_UNSIGNED_SHORT);
          break;
      case GL_R32F:
          image = cv::Mat::zeros(height, width, CV_32FC1);
          Download(image.data,  GL_RED, GL_FLOAT);
          break;

      case GL_DEPTH_COMPONENT16:
          image = cv::Mat::zeros(height, width, CV_16UC1);
          Download(image.data, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT);
          break;
      case GL_RGB8:
          image = cv::Mat::zeros(height, width, CV_8UC3);
          Download(image.data, GL_RGB, GL_UNSIGNED_BYTE);
          break;
      case GL_RGBA8:
          image = cv::Mat::zeros(height, width, CV_8UC4);
          Download(image.data, GL_RGBA, GL_UNSIGNED_BYTE);
          break;
      case GL_RGB16:
          image = cv::Mat::zeros(height, width, CV_16UC3);
          Download(image.data, GL_RGB_INTEGER_EXT, GL_UNSIGNED_SHORT);
          break;
      case GL_RGBA16:
          image = cv::Mat::zeros(height, width, CV_16UC4);
          Download(image.data, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_SHORT);
          break;
      case GL_LUMINANCE:
      case GL_LUMINANCE32F_ARB:
          image.create(height, width, CV_32FC1);
          Download(image.data, GL_LUMINANCE, GL_FLOAT);
          break;
      case GL_RGB:
      case GL_RGB32F:
          image.create(height, width, CV_32FC3);
          Download(image.data, GL_RGB, GL_FLOAT);
          break;
      case GL_RGBA:
      case GL_RGBA32F:
          image.create(height, width, CV_32FC4);
          Download(image.data, GL_RGBA, GL_FLOAT);
          break;
      default:
          std::cout << "GlTexture::Download - Unknown internal format" << std::endl;
          exit(-1);
  }
}

void GlTexture::CopyFrom(const GlTexture& tex) {
  if (!tid || width != tex.width || height != tex.height ||
      internal_format != tex.internal_format) {
    Reinitialise(tex.width, tex.height, tex.internal_format, true);
  }

  glCopyImageSubData(
      tex.tid, GL_TEXTURE_2D, 0, 0, 0, 0, tid, GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);
  CheckGlDieOnError();
}

#endif // HAVE_GLES

void GlTexture::SetLinear() {
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  Unbind();
}

void GlTexture::SetNearestNeighbour() {
  Bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  Unbind();
}


void GlTexture::RenderToViewport(const bool flip) const {
  if(flip) {
    RenderToViewportFlipY();
  }else{
    RenderToViewport();
  }
}

void GlTexture::RenderToViewport() const {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLfloat sq_vert[] = { -1,-1,  1,-1,  1, 1,  -1, 1 };
  glVertexPointer(2, GL_FLOAT, 0, sq_vert);
  glEnableClientState(GL_VERTEX_ARRAY);

  GLfloat sq_tex[]  = { 0,0,  1,0,  1,1,  0,1  };
  glTexCoordPointer(2, GL_FLOAT, 0, sq_tex);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glEnable(GL_TEXTURE_2D);
  Bind();

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    Unbind();
  glDisable(GL_TEXTURE_2D);
}

void GlTexture::RenderToViewportFlipY() const {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLfloat sq_vert[] = { -1,-1,  1,-1,  1, 1,  -1, 1 };
  glVertexPointer(2, GL_FLOAT, 0, sq_vert);
  glEnableClientState(GL_VERTEX_ARRAY);

  GLfloat sq_tex[]  = { 0,1,  1,1,  1,0,  0,0  };
  glTexCoordPointer(2, GL_FLOAT, 0, sq_tex);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glEnable(GL_TEXTURE_2D);
  Bind();

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    Unbind();
  glDisable(GL_TEXTURE_2D);
}

void GlTexture::RenderToViewportFlipXFlipY() const {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLfloat sq_vert[] = { 1,1,  -1,1,  -1,-1,  1,-1 };
  glVertexPointer(2, GL_FLOAT, 0, sq_vert);
  glEnableClientState(GL_VERTEX_ARRAY);

  GLfloat sq_tex[]  = { 0,0,  1,0,  1,1,  0,1  };
  glTexCoordPointer(2, GL_FLOAT, 0, sq_tex);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glEnable(GL_TEXTURE_2D);
  Bind();

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    Unbind();
  glDisable(GL_TEXTURE_2D);
}

const size_t datatype_bytes[] = {
    1, //  #define GL_BYTE 0x1400
    1, //  #define GL_UNSIGNED_BYTE 0x1401
    2, //  #define GL_SHORT 0x1402
    2, //  #define GL_UNSIGNED_SHORT 0x1403
    4, //  #define GL_INT 0x1404
    4, //  #define GL_UNSIGNED_INT 0x1405
    4, //  #define GL_FLOAT 0x1406
    2, //  #define GL_2_BYTES 0x1407
    3, //  #define GL_3_BYTES 0x1408
    4, //  #define GL_4_BYTES 0x1409
    8  //  #define GL_DOUBLE 0x140A
};

const size_t format_channels[] = {
    1, //  #define GL_RED 0x1903
    1, //  #define GL_GREEN 0x1904
    1, //  #define GL_BLUE 0x1905
    1, //  #define GL_ALPHA 0x1906
    3, //  #define GL_RGB 0x1907
    4, //  #define GL_RGBA 0x1908
    1, //  #define GL_LUMINANCE 0x1909
    2  //  #define GL_LUMINANCE_ALPHA 0x190A
};

size_t GlDataTypeBytes(GLenum type)
{
  return datatype_bytes[type - GL_BYTE];
}

size_t GlFormatChannels(GLenum data_layout)
{
  return format_channels[data_layout - GL_RED];
}


} // namespace cvte