//
// Created by 范帝楷 on 2020/5/29.
//

#include "../include/openglUtils/glTexture.h"

namespace FGLTools {

GlBufferData::GlBufferData()
    : bo(0) {
}

GlBufferData::GlBufferData(GlBufferType buffer_type, GLuint size_bytes, GLenum gluse,
                           const unsigned char* data )
    : bo(0) {
  Reinitialise(buffer_type, size_bytes, gluse, data );
}

void GlBufferData::swap(GlBufferData& tex) {
  std::swap(bo, tex.bo);
  std::swap(buffer_type, tex.buffer_type);
  std::swap(size_bytes, tex.size_bytes);
  std::swap(gluse, tex.gluse);
}

//! Move Constructor
GlBufferData::GlBufferData(GlBufferData&& tex)
    : bo(0) {
  *this = std::move(tex);
}

GlBufferData& GlBufferData::operator=(GlBufferData&& tex) {
  Free();
  this->bo = tex.bo;
  this->buffer_type = tex.buffer_type;
  this->gluse = tex.gluse;
  this->size_bytes = tex.size_bytes;
  tex.bo = 0;
  return *this;
}

GlBufferData::~GlBufferData() {
  Free();
}

void GlBufferData::Free() {
  if(bo!=0) {
    glDeleteBuffers(1, &bo);
  }
}

bool GlBufferData::IsValid() const {
  return bo != 0;
}

size_t GlBufferData::SizeBytes() const {
  return size_bytes;
}

void GlBufferData::Reinitialise(GlBufferType buffer_type, GLuint size_bytes, GLenum gluse,
                                const unsigned char* data ) {
  if(!bo) {
    glGenBuffers(1, &bo);
  }

  this->buffer_type = buffer_type;
  this->gluse = gluse;
  this->size_bytes = size_bytes;

  Bind();
  glBufferData(buffer_type, size_bytes, data, gluse);
  Unbind();
}

void GlBufferData::Bind() const {
  glBindBuffer(buffer_type, bo);
}

void GlBufferData::Unbind() const {
  glBindBuffer(buffer_type, 0);
}

void GlBufferData::Upload(const GLvoid* data, GLsizeiptr size_bytes, GLintptr offset) {
  if(offset + size_bytes > this->size_bytes) {
    throw std::runtime_error("GlBufferData: Trying to upload past capacity.");
  }

  Bind();
  glBufferSubData(buffer_type,offset,size_bytes,data);
  Unbind();
}

void GlBufferData::Download(GLvoid* data, GLsizeiptr size_bytes, GLintptr offset) const {
  Bind();
  glGetBufferSubData(buffer_type, offset, size_bytes, data);
  Unbind();
}


GlBuffer::GlBuffer()
    : GlBufferData(), num_elements(0)
{
}

GlBuffer::GlBuffer(GlBufferType buffer_type, GLuint num_elements, GLenum datatype, GLuint count_per_element, GLenum gluse )
    : GlBufferData(buffer_type, num_elements * count_per_element * GlDataTypeBytes(datatype), gluse),
      datatype(datatype), num_elements(num_elements), count_per_element(count_per_element)
{
}


GlBuffer::GlBuffer(GlBuffer&& o)
    : GlBufferData() {
  *this = std::move(o);
}

void GlBuffer::swap(GlBufferData& tex) {
  GlBuffer* ptex = dynamic_cast<GlBuffer*>(&tex);
  GlBufferData::swap(tex);
  if(ptex) {
    std::swap(datatype, ptex->datatype);
    std::swap(num_elements, ptex->num_elements);
    std::swap(count_per_element, ptex->count_per_element);
  }
}

GlBuffer& GlBuffer::operator=(GlBuffer&& o) {
  datatype = o.datatype;
  num_elements = o.num_elements;
  count_per_element = o.count_per_element;
  GlBufferData::operator =(std::move(o));
  return *this;
}

void GlBuffer::Reinitialise(GlBufferType buffer_type, GLuint num_elements, GLenum datatype, GLuint count_per_element, GLenum gluse, const unsigned char* data ) {
  this->datatype = datatype;
  this->num_elements = num_elements;
  this->count_per_element = count_per_element;
  const GLuint size_bytes = num_elements * count_per_element * GlDataTypeBytes(datatype);
  GlBufferData::Reinitialise(buffer_type, size_bytes, gluse, data);
}

void GlBuffer::Reinitialise(GlBuffer const& other ) {
  Reinitialise(other.buffer_type, other.num_elements, other.datatype, other.count_per_element, other.gluse);
}

void GlBuffer::Resize(GLuint new_num_elements) {
  if(bo!=0) {
#ifndef HAVE_GLES
    // Backup current data, reinit memory, restore old data
    const size_t backup_elements = std::min(new_num_elements,num_elements);
    const size_t backup_size_bytes = backup_elements*GlDataTypeBytes(datatype)*count_per_element;
    unsigned char* backup = new unsigned char[backup_size_bytes];
    Bind();
    glGetBufferSubData(buffer_type, 0, backup_size_bytes, backup);
    glBufferData(buffer_type, new_num_elements*GlDataTypeBytes(datatype)*count_per_element, 0, gluse);
    glBufferSubData(buffer_type, 0, backup_size_bytes, backup);
    Unbind();
    delete[] backup;
#else
    throw std::exception();
#endif
  }else{
    Reinitialise(buffer_type, new_num_elements, datatype, count_per_element, gluse);
  }
  num_elements = new_num_elements;
}

GLArrayBuffer::GLArrayBuffer() {
  vao = 0;
}
GLArrayBuffer::GLArrayBuffer(GlBufferType buffer_type, GLuint num_elements, GLenum datatype,
  	    GLuint count_per_element, GLenum gluse) 
        : GlBuffer(buffer_type, num_elements, datatype, count_per_element, gluse) {
    glGenVertexArrays(1,&vao);
}

GLArrayBuffer::GLArrayBuffer(GLArrayBuffer&& tex) 
  : GlBuffer(std::move(tex)){
  vao = tex.vao;
  tex.vao = 0;
}

void GLArrayBuffer::swap(GlBufferData& tex) {
  GlBuffer::swap(tex);
  GLArrayBuffer* ptex = dynamic_cast<GLArrayBuffer*>(&tex);
  if(ptex){
    std::swap(vao, ptex->vao);
    std::swap(pointAttrSize, ptex->pointAttrSize);
  }
}

void GLArrayBuffer::Bind() const {
  GlBuffer::Bind();
  glBindVertexArray(vao);
}

void GLArrayBuffer::Unbind() const {
  glBindVertexArray(0);
  GlBuffer::Unbind();
}

void GLArrayBuffer::Reinitialise(GlBufferType buffer_type, GLuint num_elements, GLenum datatype, GLuint
        count_per_element, GLenum gluse, const unsigned char* data) {
    if(vao == 0)
      glDeleteVertexArrays(1,&vao);
    GlBuffer::Reinitialise(buffer_type,num_elements,datatype, count_per_element, gluse, data);
    glGenVertexArrays(1,&vao);
}

GLArrayBuffer::~GLArrayBuffer() {
  if(vao == 0) {
    glDeleteVertexArrays(1,&vao);
  }
}

void GLArrayBuffer::Reinitialise(GLArrayBuffer const& other) {
  if(vao == 0)
      glDeleteVertexArrays(1,&vao);
  GlBuffer::Reinitialise(other);
  glGenVertexArrays(1,&vao);
}

void GLArrayBuffer::Resize(GLuint num_elements) {
  if(vao == 0)
      glDeleteVertexArrays(1,&vao);
  GlBuffer::Resize(num_elements);
  glGenVertexArrays(1,&vao);
}

void GLArrayBuffer::setVertexAttribPointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
  glVertexAttribPointer(pointAttrSize, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(pointAttrSize);
  pointAttrSize++;
}

void GLArrayBuffer::resetVertexAttribPointer() {
  for(GLuint i = 0; i < pointAttrSize; ++i) {
      glDisableVertexAttribArray(i);  
  }
  pointAttrSize = 0;
}

GlSizeableBuffer::GlSizeableBuffer(GlBufferType buffer_type, GLuint initial_num_elements, GLenum datatype, GLuint count_per_element, GLenum gluse )
    : GlBuffer(buffer_type, initial_num_elements, datatype, count_per_element, gluse), m_num_verts(0) {

}

void GlSizeableBuffer::Clear() {
  m_num_verts = 0;
}

size_t GlSizeableBuffer::start() const {
  return 0;
}

size_t GlSizeableBuffer::size() const {
  return m_num_verts;
}

void GlSizeableBuffer::CheckResize(size_t num_verts) {
  if( num_verts > GlBuffer::num_elements) {
    const size_t new_size = NextSize(num_verts);
    GlBuffer::Resize((GLuint)new_size);
  }
}

size_t GlSizeableBuffer::NextSize(size_t min_size) const {
  size_t new_size = std::max(GlBuffer::num_elements, 1u);
  while(new_size < min_size) {
    new_size *= 2;
  }
  return new_size;
}


}