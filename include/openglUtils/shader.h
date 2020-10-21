//
// Created by 范帝楷 on 2020/6/6.
//

#ifndef FGPUTILS_PROJECT_SHADER_H
#define FGPUTILS_PROJECT_SHADER_H

#include "glsl.h"
#include <memory>
#include "uniform.h"

namespace FGLTools {

class Shader : public GlSlProgram {
public:
	Shader();
	GLuint programId();
	void setUniform(const FGLTools::Uniform& v);
};

std::shared_ptr<Shader> loadProgramGeomFromFile(
	const std::string& path,
	const std::string& vertex_shader_file,
	const std::string& geometry_shader_file);

std::shared_ptr<Shader> loadProgramFromFile(const std::string& path,
	const std::string& vertex_shader_file);

std::shared_ptr<Shader> loadProgramFromFile(
	const std::string& path,
	const std::string& vertex_shader_file,
	const std::string& fragment_shader_file);

std::shared_ptr<Shader> loadProgramFromFile(
	const std::string& path,
	const std::string& vertex_shader_file,
	const std::string& fragment_shader_file,
	const std::string& geometry_shader_file);
}

#endif //FGPUTILS_PROJECT_SHADER_H
