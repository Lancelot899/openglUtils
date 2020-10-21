//
// Created by 范帝楷 on 2020/5/29.
//

#pragma once

#include <GL/glew.h>

#ifdef HAVE_GLES
#if defined(_ANDROID_)
        #include <EGL/egl.h>
        #ifdef HAVE_GLES_2
            #include <GLES2/gl2.h>
            #include <GLES2/gl2ext.h>
        #else
            #include <GLES/gl.h>
            #define GL_GLEXT_PROTOTYPES
            #include <GLES/glext.h>
        #endif
    #elif defined(_APPLE_IOS_)
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #endif
#else
#ifdef MAC_OS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif // HAVE_GLES

