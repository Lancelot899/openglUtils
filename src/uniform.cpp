//
// Created by 范帝楷 on 2020/6/6.
//

#include "../include/openglUtils/uniform.h"


namespace FGLTools {
	Uniform::Uniform(const std::string& id, const int& v) : id(id), i(v), t(INT) {}

	Uniform::Uniform(const std::string& id, const float& v) : id(id), f(v), t(FLOAT) {}

	Uniform::Uniform(const std::string& id, const Eigen::Vector2f& v) : id(id), v2(v), t(VEC2) {}

	Uniform::Uniform(const std::string& id, const Eigen::Vector3f& v) : id(id), v3(v), t(VEC3) {}

	Uniform::Uniform(const std::string& id, const Eigen::Vector4f& v) : id(id), v4(v), t(VEC4) {}

	Uniform::Uniform(const std::string& id, const Eigen::Matrix4f& v) : id(id), m4(v), t(MAT4) {}

	Uniform::Uniform(const std::string& id, const Eigen::Matrix3f& v) : id(id), m3(v), t(MAT3) {}
}