//
// Created by 范帝楷 on 2020/5/29.
//

#pragma once

#include "glformat_traits.h"

#include "opencv2/opencv.hpp"
#include <iostream>

namespace FGLTools {

struct GlPixFormat
{
  GlPixFormat() {}

  GlPixFormat(const cv::Mat &image)
  {
    switch(image.channels()) {
      case 1: glformat = GL_RED; break;
      case 3: glformat =  GL_RGB; break;
      case 4: glformat =  GL_RGBA; break;
      default: std::cout << "Unable to form OpenGL format from video format\n";
    }


    const bool is_integral = image.type() != CV_32FC1 && image.type() != CV_32FC3 && image.type() != CV_32FC4
                             && image.type() != CV_64FC1 && image.type() != CV_64FC3 && image.type() != CV_64FC4;
    int bit = image.elemSize1() * 8;
    switch (bit) {
      case 8: gltype = GL_UNSIGNED_BYTE; break;
      case 16: gltype = GL_UNSIGNED_SHORT; break;
      case 32: gltype = (is_integral ? GL_UNSIGNED_INT : GL_FLOAT); break;
      //case 64: gltype = (is_integral ? GL_UNSIGNED_INT64 : GL_DOUBLE); break;
      default: std::cout << "Unknown OpenGL data type for video format\n";
    }

    if(glformat == GL_RED) {
      if(gltype == GL_UNSIGNED_BYTE) {
        scalable_internal_format = GL_R8;
      }else if(gltype == GL_UNSIGNED_SHORT){
        scalable_internal_format = GL_DEPTH_COMPONENT16;
      }else{
        scalable_internal_format = GL_LUMINANCE32F_ARB;
      }
    } else if(glformat == GL_RGB) {
      if(gltype == GL_UNSIGNED_BYTE) {
        scalable_internal_format = GL_RGB8;
      }else if(gltype == GL_UNSIGNED_SHORT) {
        scalable_internal_format = GL_RGB16;
      }else{
        scalable_internal_format = GL_RGB32F;
      }
    } else{
      if(gltype == GL_UNSIGNED_BYTE) {
        scalable_internal_format = GL_RGBA8;
      }else if(gltype == GL_UNSIGNED_SHORT) {
        scalable_internal_format = GL_RGBA16;
      }else{
        scalable_internal_format = GL_RGBA32F;
      }
    }
    if(gltype == GL_UNSIGNED_SHORT) {
        glformat = GL_DEPTH_COMPONENT;
    }

  }


    GlPixFormat(const cv::Mat &image,bool need_normal)
    {
        switch(image.channels()) {
            case 1: glformat = GL_LUMINANCE; break;
            case 3: glformat =  GL_RGB; break;
            case 4: glformat =  GL_RGBA; break;
            default: std::cout << "Unable to form OpenGL format from video format\n";
        }


        const bool is_integral = image.type() != CV_32FC1 && image.type() != CV_32FC3 && image.type() != CV_32FC4
                                 && image.type() != CV_64FC1 && image.type() != CV_64FC3 && image.type() != CV_64FC4;
        int bit = image.elemSize1() * 8;
        switch (bit) {
            case 8: gltype = GL_UNSIGNED_BYTE; break;
            case 16: gltype = GL_UNSIGNED_SHORT; break;
            case 32: gltype = (is_integral ? GL_UNSIGNED_INT : GL_FLOAT); break;
                //case 64: gltype = (is_integral ? GL_UNSIGNED_INT64 : GL_DOUBLE); break;
            default: std::cout << "Unknown OpenGL data type for video format\n";
        }
if(need_normal) {
    if (glformat == GL_LUMINANCE) {
        if (gltype == GL_UNSIGNED_BYTE) {
            scalable_internal_format = GL_LUMINANCE8;
        } else if (gltype == GL_UNSIGNED_SHORT) {
            std::cout<<__FILE__<<__LINE__<<" please consifer carefully? you  want  a normalized GL_UNSIGNED_SHORT?"<<std::endl;
            scalable_internal_format = GL_DEPTH_COMPONENT16;
        } else {

            std::cout<<__FILE__<<__LINE__<<" please consifer carefully? you  want  a normalized GL_FLOAT?"<<std::endl;
            assert(gltype != GL_FLOAT);
            scalable_internal_format = GL_RED;
        }
    } else if (glformat == GL_RGB) {
        if (gltype == GL_UNSIGNED_BYTE) {
            scalable_internal_format = GL_RGB8;
        } else if (gltype == GL_UNSIGNED_SHORT) {
            std::cout<<__FILE__<<__LINE__<<" please consifer carefully? you  want  a normalized GL_UNSIGNED_SHORT?"<<std::endl;
            scalable_internal_format = GL_RGB16;
        } else {

            std::cout<<__FILE__<<__LINE__<<" please consifer carefully? you  want  a normalized GL_FLOAT?"<<std::endl;
            assert(gltype != GL_FLOAT);
            scalable_internal_format = GL_RGB32F;
        }
    } else {

        if (gltype == GL_UNSIGNED_BYTE) {
            scalable_internal_format = GL_RGBA8;
        } else if (gltype == GL_UNSIGNED_SHORT) {
            scalable_internal_format = GL_RGBA16;
        } else {
            assert(gltype == GL_FLOAT);
            scalable_internal_format = GL_RGBA32F;
        }
    }
    if (gltype == GL_UNSIGNED_SHORT) {
        glformat = GL_DEPTH_COMPONENT;
    }else{
        if(gltype == GL_FLOAT){
            glformat =  GL_RED;
        }
    }
}else{

    if (glformat == GL_LUMINANCE) {
        if (gltype == GL_UNSIGNED_BYTE) {
            scalable_internal_format = GL_R8UI;
        } else if (gltype == GL_UNSIGNED_SHORT) {
            scalable_internal_format = GL_R16UI ;
        } else {
            assert(gltype==GL_FLOAT);
            scalable_internal_format = GL_R32F;
        }
    } else if (glformat == GL_RGB) {
        if (gltype == GL_UNSIGNED_BYTE) {
            scalable_internal_format = GL_RGB8UI;
        } else if (gltype == GL_UNSIGNED_SHORT) {
            scalable_internal_format = GL_RGB16UI;
        } else {
            assert(gltype == GL_FLOAT);
            scalable_internal_format = GL_RGB32F;
        }
    } else {
        if (gltype == GL_UNSIGNED_BYTE) {
            scalable_internal_format = GL_RGBA8UI;
        } else if (gltype == GL_UNSIGNED_SHORT) {
            scalable_internal_format = GL_RGBA16UI;
        } else {
            assert(gltype == GL_FLOAT);
            scalable_internal_format = GL_RGBA32F;
        }
    }
    if (gltype == GL_UNSIGNED_SHORT) {
        glformat =  GL_RED_INTEGER;
    }else{
        if(gltype ==  GL_FLOAT){
            glformat =  GL_RED;
        }
    }





}
    }
  template<typename T>
  static GlPixFormat FromType()
  {
    GlPixFormat fmt;
    fmt.glformat = GlFormatTraits<T>::glformat;
    fmt.gltype = GlFormatTraits<T>::gltype;
    fmt.scalable_internal_format = GlFormatTraits<T>::glinternalformat;
    return fmt;
  }

  GLint glformat;
  GLenum gltype;
  GLint scalable_internal_format;
};
}

