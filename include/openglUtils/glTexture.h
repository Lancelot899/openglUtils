//
// Created by 范帝楷 on 2020/5/29.
//
#pragma once

#include <opencv2/opencv.hpp>
#include <Eigen/Core>

#include "glPlatform.h"

#define CheckGlDieOnError() FGLTools::_CheckGlDieOnError( __FILE__, __LINE__ );

namespace FGLTools {


void _CheckGlDieOnError( const char *sFile, const int nLine );
//extern const size_t datatype_bytes[];
//extern const size_t format_channels[];

class GlTexture {
 public:
  //! internal_format normally one of GL_RGBA8, GL_LUMINANCE8, GL_INTENSITY16
  GlTexture(
      GLint width,
      GLint height,
      GLint internal_format = GL_RGBA8,
      bool sampling_linear = true,
      int border = 0,
      GLenum glformat = GL_RGBA,
      GLenum gltype = GL_UNSIGNED_BYTE,
      GLvoid* data = NULL);

  // Construct this texture from a CPU image
  GlTexture(const cv::Mat& img, bool sampling_linear = true);
  GlTexture(const cv::Mat& img,bool Need_Normal, bool sampling_linear);
  //! Move Constructor / asignment
  GlTexture(GlTexture&& tex);
  GlTexture& operator=(GlTexture&& tex);

  //! Default constructor represents 'no texture'
  GlTexture();
  virtual ~GlTexture();

  bool IsValid() const;

  //! Delete OpenGL resources and fall back to representing 'no texture'
  void Delete();

  //! Reinitialise teture width / height / format
  virtual void Reinitialise(
      GLsizei width,
      GLsizei height,
      GLint internal_format = GL_RGBA8,
      bool sampling_linear = true,
      int border = 0,
      GLenum glformat = GL_RGBA,
      GLenum gltype = GL_UNSIGNED_BYTE,
      GLvoid* data = NULL);

  void Bind() const;
  void Unbind() const;

  //! data_layout normally one of GL_LUMINANCE, GL_RGB, ...
  //! data_type normally one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_FLOAT
  void Upload(const void* image, GLenum data_format = GL_LUMINANCE, GLenum data_type = GL_FLOAT);

  //! Upload data to texture, overwriting a sub-region of it.
  //! data ptr contains packed data_w x data_h of pixel data.
  void Upload(
      const void* data,
      GLsizei tex_x_offset,
      GLsizei tex_y_offset,
      GLsizei data_w,
      GLsizei data_h,
      GLenum data_format,
      GLenum data_type);

  void Load(const cv::Mat& img, bool sampling_linear = true);
//CV_MAT_TYPE
  void Load(const cv::Mat& img,bool Need_Normal, bool sampling_linear);
  void Download(void* image, GLenum data_layout = GL_LUMINANCE, GLenum data_type = GL_FLOAT) const;

  void Download(cv::Mat& img) const;

  void CopyFrom(const GlTexture& tex);

  void SetLinear();
  void SetNearestNeighbour();

  void RenderToViewport(const bool flip) const;
  void RenderToViewport() const;
  void RenderToViewportFlipY() const;
  void RenderToViewportFlipXFlipY() const;

  GLint internal_format = 0;
  GLuint tid = 0;
  GLint width = 0;
  GLint height = 0;

 private:
  // Private copy constructor
  GlTexture(const GlTexture&) = delete;
};

struct GlRenderBuffer {
    GlRenderBuffer();
    GlRenderBuffer(GLint width, GLint height, GLint internal_format = GL_DEPTH_COMPONENT24);

    void Reinitialise(GLint width, GLint height, GLint internal_format = GL_DEPTH_COMPONENT24);

    //! Move Constructor
    GlRenderBuffer(GlRenderBuffer&& tex);

    ~GlRenderBuffer();

    GLint width;
    GLint height;
    GLuint rbid;

   private:
    GlRenderBuffer(const GlRenderBuffer&) = delete;
};

struct GlFramebuffer {
  GlFramebuffer();
  ~GlFramebuffer();

  GlFramebuffer(GlTexture& colour, GlRenderBuffer& depth);
  GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlRenderBuffer& depth);
  GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlTexture& colour2, GlRenderBuffer& depth);
  GlFramebuffer(GlTexture& colour0, GlTexture& colour1, GlTexture& colour2, GlTexture& colour3, GlRenderBuffer& depth);

  void Bind() const;
  void Unbind() const;

  void Reinitialise();

  // Attach Colour texture to frame buffer
  // Return attachment texture is bound to (e.g. GL_COLOR_ATTACHMENT0_EXT)
  GLenum AttachColour(GlTexture& tex);

  // Attach Depth render buffer to frame buffer
  void AttachDepth(GlRenderBuffer& rb);

  GLuint fbid;
  unsigned attachments;
};

 enum GlBufferType
{
  GlUndefined = 0,
  GlArrayBuffer = GL_ARRAY_BUFFER,                    // VBO's, CBO's, NBO's
  GlElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,     // IBO's
#ifndef HAVE_GLES
  GlPixelPackBuffer = GL_PIXEL_PACK_BUFFER,           // PBO's
  GlPixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
  GlShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER
#endif
};

// This encapsulates a GL Buffer object.
struct  GlBufferData {
  //! Default constructor represents 'no buffer'
  GlBufferData();
  GlBufferData(GlBufferType buffer_type, GLuint size_bytes, GLenum gluse = GL_DYNAMIC_DRAW, const
  unsigned char* data = 0 ); 
  virtual ~GlBufferData(); 
  void Free();

  //! Move Constructor
  GlBufferData(GlBufferData&& tex);
  GlBufferData& operator=(GlBufferData&& tex);

  bool IsValid() const;

  size_t SizeBytes() const;
  virtual void swap(GlBufferData& tex);

  virtual void Reinitialise(GlBufferType buffer_type, GLuint size_bytes, GLenum gluse = GL_DYNAMIC_DRAW,
  const unsigned char* data = 0 );

  virtual void Bind() const;
  virtual void Unbind() const;
  void Upload(const GLvoid* data, GLsizeiptr size_bytes, GLintptr offset = 0);
  void Download(GLvoid* ptr, GLsizeiptr size_bytes, GLintptr offset = 0) const;

  GLuint bo;
  GlBufferType buffer_type;
  GLenum gluse;
  GLuint size_bytes;

 private:
  GlBufferData(const GlBufferData&) {}
};

 struct GlBuffer : public GlBufferData
{
  //! Default constructor represents 'no buffer'
  GlBuffer();
  GlBuffer(GlBufferType buffer_type, GLuint num_elements, GLenum datatype,
  	GLuint count_per_element, GLenum gluse = GL_DYNAMIC_DRAW );

  GlBuffer(const GlBuffer&) = delete;

  //! Move Constructor
  GlBuffer(GlBuffer&& tex);
  GlBuffer& operator=(GlBuffer&& tex);
  
  virtual void Reinitialise(GlBufferType buffer_type, GLuint num_elements, GLenum datatype, GLuint
        count_per_element, GLenum gluse, const unsigned char* data = nullptr );
  virtual void Reinitialise(GlBuffer const& other );
  virtual void Resize(GLuint num_elements);
  virtual void swap(GlBufferData& tex);
  GLenum datatype;
  GLuint num_elements;
  GLuint count_per_element;
};

struct GLArrayBuffer : public GlBuffer{
  GLArrayBuffer();
  ~GLArrayBuffer();
  GLArrayBuffer(GlBufferType buffer_type, GLuint num_elements, GLenum datatype,
  	    GLuint count_per_element, GLenum gluse = GL_DYNAMIC_DRAW );
  GLArrayBuffer(GLArrayBuffer&& tex);
  GLArrayBuffer(const GLArrayBuffer&) = delete;
  void Reinitialise(GlBufferType buffer_type, GLuint num_elements, GLenum datatype, GLuint
        count_per_element, GLenum gluse, const unsigned char* data = nullptr );
  void Reinitialise(GLArrayBuffer const& other);
  void Resize(GLuint num_elements);
  void setVertexAttribPointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
  void resetVertexAttribPointer();
  void Bind() const;
  void Unbind() const;
  virtual void swap(GlBufferData& tex);
  GLuint vao;
  GLuint pointAttrSize = 0;
};

 class GlSizeableBuffer
    : public GlBuffer {
 public:
  GlSizeableBuffer(GlBufferType buffer_type, GLuint initial_num_elements, GLenum
  datatype, GLuint count_per_element, GLenum gluse = GL_DYNAMIC_DRAW );

  void Clear();

  template<typename Derived>
  void Add(const Eigen::DenseBase<Derived>& vec) {
      typedef typename Eigen::DenseBase<Derived>::Scalar Scalar;
      assert(vec.rows()==GlBuffer::count_per_element);
      CheckResize(m_num_verts + 1);
      // TODO: taking address of first element is really dodgey. Need to work out
      // when this is okay!
      Upload(&vec(0,0), sizeof(Scalar)*vec.rows()*vec.cols(), sizeof(Scalar)*vec.rows()*m_num_verts);
      m_num_verts += vec.cols();
  }

  template<typename Derived>
  void Update(const Eigen::DenseBase<Derived>& vec, size_t position = 0) {
      typedef typename Eigen::DenseBase<Derived>::Scalar Scalar;
      assert(vec.rows()==GlBuffer::count_per_element);
      CheckResize(position + vec.cols() );
      // TODO: taking address of first element is really dodgey. Need to work out
      // when this is okay!
      Upload(&vec(0,0), sizeof(Scalar)*vec.rows()*vec.cols(), sizeof(Scalar)*vec.rows()*position );
      m_num_verts = std::max(position+vec.cols(), m_num_verts);
  }

  size_t start() const;

  size_t size() const;

 protected:
  void CheckResize(size_t num_verts);

  size_t NextSize(size_t min_size) const;

  size_t  m_num_verts;
};

size_t GlFormatChannels(GLenum data_layout);

size_t GlDataTypeBytes(GLenum type);

} // namespace FGLTools
