//
// Created by 范帝楷 on 2020/6/1.
//


#pragma once

#include "./glPlatform.h"
#include <vector>

#include "Eigen/Core"
#include "Eigen/Geometry"


namespace FGLTools {

#ifdef HAVE_GLES
typedef float GLprecision;
#else
typedef double GLprecision;
#endif

/// Capture OpenGL matrix types in enum to typing.
enum OpenGlStack {
  GlModelViewStack =  0x1700, // GL_MODELVIEW
  GlProjectionStack = 0x1701, // GL_PROJECTION
  GlTextureStack =    0x1702  // GL_TEXTURE
};

enum AxisDirection
{
  AxisNone,
  AxisNegX, AxisX,
  AxisNegY, AxisY,
  AxisNegZ, AxisZ
};

struct CameraSpec {
  GLprecision forward[3];
  GLprecision up[3];
  GLprecision right[3];
  GLprecision img_up[2];
  GLprecision img_right[2];
};

const static CameraSpec CameraSpecOpenGl = {{0,0,-1},{0,1,0},{1,0,0},{0,1},{1,0}};

const static CameraSpec CameraSpecYDownZForward = {{0,0,1},{0,-1,0},{1,0,0},{0,-1},{1,0}};

/// Direction vector for each AxisDirection enum
const static GLprecision AxisDirectionVector[7][3] = {
    {0,0,0},
    {-1,0,0}, {1,0,0},
    {0,-1,0}, {0,1,0},
    {0,0,-1}, {0,0,1}
};

/// Object representing OpenGl Matrix.
struct OpenGlMatrix {
  static OpenGlMatrix Translate(GLprecision x, GLprecision y, GLprecision z);
  static OpenGlMatrix Scale(GLprecision x, GLprecision y, GLprecision z);
  static OpenGlMatrix RotateX(GLprecision theta_rad);
  static OpenGlMatrix RotateY(GLprecision theta_rad);
  static OpenGlMatrix RotateZ(GLprecision theta_rad);


  template<typename P>
  static OpenGlMatrix ColMajor4x4(const P* col_major_4x4);

  OpenGlMatrix();

  template<typename P>
  OpenGlMatrix(const Eigen::Matrix<P,4,4>& mat);

  template<typename P>
  OpenGlMatrix(const Eigen::Transform<P,3,Eigen::Affine>& mat) : OpenGlMatrix(mat.matrix()) { }

  template<typename P>
  operator Eigen::Matrix<P,4,4>() const;

  template<typename P>
  operator Eigen::Transform<P,3,Eigen::Affine>() const;

  // Load matrix on to OpenGl stack
  void Load() const;

  void Multiply() const;

  void SetIdentity();

  OpenGlMatrix Transpose() const;

  OpenGlMatrix Inverse() const;

  GLprecision& operator()(int r, int c) {
    return m[4*c +r];
  }

  GLprecision operator()(int r, int c) const {
    return m[4 * c + r];
  }

  // Column major Internal buffer
  GLprecision m[16];
};

OpenGlMatrix operator*(const OpenGlMatrix& lhs, const OpenGlMatrix& rhs);

std::ostream& operator<<(std::ostream& os, const OpenGlMatrix& mat);

/// Deprecated.
struct  OpenGlMatrixSpec : public OpenGlMatrix {
  // Specify which stack this refers to
  OpenGlStack type;
};

/// Object representing attached OpenGl Matrices / transforms.
class OpenGlRenderState
{
 public:
  OpenGlRenderState();
  OpenGlRenderState(const OpenGlMatrix& projection_matrix);
  OpenGlRenderState(const OpenGlMatrix& projection_matrix, const OpenGlMatrix& modelview_matrix);

  static void ApplyIdentity();

  void Apply() const;
  OpenGlRenderState& SetProjectionMatrix(OpenGlMatrix m);
  OpenGlRenderState& SetModelViewMatrix(OpenGlMatrix m);

  OpenGlMatrix& GetProjectionMatrix();
  OpenGlMatrix GetProjectionMatrix() const;

  OpenGlMatrix& GetModelViewMatrix();
  OpenGlMatrix GetModelViewMatrix() const;

  OpenGlMatrix GetProjectionModelViewMatrix() const;
  OpenGlMatrix GetProjectiveTextureMatrix() const;

  void EnableProjectiveTexturing() const;
  void DisableProjectiveTexturing() const;

  //! Seemlessly move OpenGl camera relative to changes in T_wc,
  //! whilst still enabling interaction
  void Follow(const OpenGlMatrix& T_wc, bool follow = true);
  void Unfollow();

  // Experimental - subject to change
  OpenGlMatrix& GetProjectionMatrix(unsigned int view);
  OpenGlMatrix GetProjectionMatrix(unsigned int view) const;
  OpenGlMatrix& GetViewOffset(unsigned int view);
  OpenGlMatrix GetViewOffset(unsigned int view) const;
  OpenGlMatrix GetModelViewMatrix(int i) const;
  void ApplyNView(int view) const;

  OpenGlRenderState& Set(OpenGlMatrixSpec spec);

 protected:
  OpenGlMatrix modelview;
  std::vector<OpenGlMatrix> projection;
  std::vector<OpenGlMatrix> modelview_premult;
  OpenGlMatrix T_cw;
  bool follow;
};

// Camera Axis:
//   X - Right, Y - Up, Z - Back
// Image Origin:
//   Bottom Left
// Caution: Principal point defined with respect to image origin (0,0) at
//          top left of top-left pixel (not center, and in different frame
//          of reference to projection function image)
OpenGlMatrixSpec ProjectionMatrixRUB_BottomLeft(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

// Camera Axis:
//   X - Right, Y - Up, Z - Back
// Image Origin:
//   Top Left
// Caution: Principal point defined with respect to image origin (0,0) at
//          top left of top-left pixel (not center, and in different frame
//          of reference to projection function image)
OpenGlMatrixSpec ProjectionMatrixRUB_TopLeft(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

// Camera Axis:
//   X - Right, Y - Down, Z - Forward
// Image Origin:
//   Top Left
// Pricipal point specified with image origin (0,0) at top left of top-left pixel (not center)
OpenGlMatrixSpec ProjectionMatrixRDF_TopLeft(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

// Camera Axis:
//   X - Right, Y - Down, Z - Forward
// Image Origin:
//   Top Right
// Pricipal point specified with image origin (0,0) at top right of top-right pixel (not center)
OpenGlMatrixSpec ProjectionMatrixRDF_TopRight(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

// Camera Axis:
//   X - Right, Y - Down, Z - Forward
// Image Origin:
//   Bottom Left
// Pricipal point specified with image origin (0,0) at top left of top-left pixel (not center)
OpenGlMatrixSpec ProjectionMatrixRDF_BottomLeft(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

// Camera Axis:
//   X - Right, Y - Down, Z - Forward
// Image Origin:
//   Bottom Right
// Pricipal point specified with image origin (0,0) at top right of top-right pixel (not center)
OpenGlMatrixSpec ProjectionMatrixRDF_BottomRight(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

//! Use OpenGl's default frame RUB_BottomLeft
 
OpenGlMatrixSpec ProjectionMatrix(int w, int h, GLprecision fu, GLprecision fv, GLprecision u0, GLprecision v0, GLprecision zNear, GLprecision zFar );

 
OpenGlMatrixSpec ProjectionMatrixOrthographic(GLprecision l, GLprecision r, GLprecision b, GLprecision t, GLprecision n, GLprecision f );


//! Generate glulookat style model view matrix, looking at (lx,ly,lz)
//! X-Right, Y-Up, Z-Back
 
OpenGlMatrix ModelViewLookAtRUB(GLprecision ex, GLprecision ey, GLprecision ez, GLprecision lx, GLprecision ly, GLprecision lz, GLprecision ux, GLprecision uy, GLprecision uz);

//! Generate glulookat style model view matrix, looking at (lx,ly,lz)
//! X-Right, Y-Down, Z-Forward
 
OpenGlMatrix ModelViewLookAtRDF(GLprecision ex, GLprecision ey, GLprecision ez, GLprecision lx, GLprecision ly, GLprecision lz, GLprecision ux, GLprecision uy, GLprecision uz);

//! Generate glulookat style model view matrix, OpenGL Default camera convention (XYZ=RUB), looking at (lx,ly,lz)
 
OpenGlMatrix ModelViewLookAt(GLprecision x, GLprecision y, GLprecision z, GLprecision lx, GLprecision ly, GLprecision lz, AxisDirection up);

 
OpenGlMatrix ModelViewLookAt(GLprecision ex, GLprecision ey, GLprecision ez, GLprecision lx, GLprecision ly, GLprecision lz, GLprecision ux, GLprecision uy, GLprecision uz);

OpenGlMatrix ModelViewlookAtRH_1(const Eigen::Vector3f & eye, const Eigen::Vector3f& center,  const Eigen::Vector3f& up);

OpenGlMatrix ModelViewlookAtRH(const Eigen::Vector3f & eye, const Eigen::Vector3f& center,  const Eigen::Vector3f& up);
OpenGlMatrix IdentityMatrix();

 
OpenGlMatrixSpec IdentityMatrix(OpenGlStack type);

 
OpenGlMatrixSpec negIdentityMatrix(OpenGlStack type);


template<typename P>
Eigen::Matrix<P,4,4> ToEigen(const OpenGlMatrix& ms);

}

// Inline definitions
namespace FGLTools
{

template<typename P>
inline OpenGlMatrix OpenGlMatrix::ColMajor4x4(const P* col_major_4x4)
{
  OpenGlMatrix mat;
  std::copy(col_major_4x4, col_major_4x4 + 16, mat.m);
  return mat;
}

inline OpenGlMatrix::OpenGlMatrix() {
}

template<typename P> inline
OpenGlMatrix::OpenGlMatrix(const Eigen::Matrix<P,4,4>& mat)
{
  for(int r=0; r<4; ++r ) {
    for(int c=0; c<4; ++c ) {
      m[c*4+r] = mat(r,c);
    }
  }
}

template<typename P>
OpenGlMatrix::operator Eigen::Matrix<P,4,4>() const
{
  return ToEigen<P>(*this);
}

template<typename P>
OpenGlMatrix::operator Eigen::Transform<P,3,Eigen::Affine>() const
{
  return Eigen::Transform<P,3,Eigen::Affine>(ToEigen<P>(*this));
}

template<typename P> inline
Eigen::Matrix<P,4,4> ToEigen(const OpenGlMatrix& ms)
{
  Eigen::Matrix<P,4,4> mat;
  for(int r=0; r<4; ++r ) {
    for(int c=0; c<4; ++c ) {
      mat(r,c) = (P)ms.m[c*4+r];
    }
  }
  return mat;
}


}
