//
// Created by 范帝楷 on 2020/6/6.
//

#ifndef FGPUTILS_PROJECT_UNIFORM_H
#define FGPUTILS_PROJECT_UNIFORM_H

#include <Eigen/Core>
#include <string>


namespace FGLTools {

	class Uniform {
	public:
		Uniform(const std::string& id, const int& v);
		Uniform(const std::string& id, const float& v);
		Uniform(const std::string& id, const Eigen::Vector2f& v);
		Uniform(const std::string& id, const Eigen::Vector3f& v);
		Uniform(const std::string& id, const Eigen::Vector4f& v);
		Uniform(const std::string& id, const Eigen::Matrix4f& v);
		Uniform(const std::string& id, const Eigen::Matrix3f& v);
		std::string id;

		int i;
		float f;
		Eigen::Vector2f v2;
		Eigen::Vector3f v3;
		Eigen::Vector4f v4;
		Eigen::Matrix4f m4;
		Eigen::Matrix3f m3;

		enum Type { INT, FLOAT, VEC2, VEC3, VEC4, MAT4,MAT3, NONE };

		Type t;
	};
}


#endif //FGPUTILS_PROJECT_UNIFORM_H
