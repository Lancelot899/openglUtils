//
// Created by 范帝楷 on 2020/6/6.
//

#include "../include/openglUtils/shader.h"

namespace FGLTools {
	Shader::Shader() {}

	GLuint Shader::programId() {
		return prog;
	}

	void Shader::setUniform(const FGLTools::Uniform& v) {
		GLuint loc = glGetUniformLocation(prog, v.id.c_str());

		switch (v.t) {
			case Uniform::INT:
				glUniform1i(loc, v.i);
				break;
			case Uniform::FLOAT:
				glUniform1f(loc, v.f);
				break;
			case Uniform::VEC2:
				glUniform2f(loc, v.v2(0), v.v2(1));
				break;
			case Uniform::VEC3:
				glUniform3f(loc, v.v3(0), v.v3(1), v.v3(2));
				break;
			case Uniform::VEC4:
				glUniform4f(loc, v.v4(0), v.v4(1), v.v4(2), v.v4(3));
				break;
			case Uniform::MAT4:
				glUniformMatrix4fv(loc, 1, false, v.m4.data());
				break;
			case Uniform::MAT3:
				glUniformMatrix3fv(loc, 1, false, v.m3.data());
				break;

			default:
				assert(false && "Uniform type not implemented!");
				break;
		}
	}

	std::shared_ptr<Shader> loadProgramGeomFromFile(
		const std::string& path,
		const std::string& vertex_shader_file,
		const std::string& geometry_shader_file) {
		std::shared_ptr<Shader> program = std::make_shared<Shader>();

		program->AddShaderFromFile(
			FGLTools::GlSlVertexShader,
			path + "/" +  vertex_shader_file,
			{},
			{path});
		program->AddShaderFromFile(
			FGLTools::GlSlGeometryShader,
			path + "/" + geometry_shader_file,
			{},
			{path});
		program->Link();

		return program;
	}

	std::shared_ptr<Shader> loadProgramFromFile(const std::string &path, const std::string& vertex_shader_file) {
		std::shared_ptr<Shader> program = std::make_shared<Shader>();

		program->AddShaderFromFile(
			FGLTools::GlSlVertexShader,
			path + "/" + vertex_shader_file,
			{},
			{path});
		program->Link();

		return program;
	}

	std::shared_ptr<Shader> loadProgramFromFile(
		const std::string& path,
		const std::string& vertex_shader_file,
		const std::string& fragment_shader_file) {
		std::shared_ptr<Shader> program = std::make_shared<Shader>();

		program->AddShaderFromFile(
			FGLTools::GlSlVertexShader,
			path + "/" + vertex_shader_file,
			{},
			{path});
		program->AddShaderFromFile(
			FGLTools::GlSlFragmentShader,
			path + "/" + fragment_shader_file,
			{},
			{path});
		program->Link();

		return program;
	}

	std::shared_ptr<Shader> loadProgramFromFile(
		const std::string& path,
		const std::string& vertex_shader_file,
		const std::string& fragment_shader_file,
		const std::string& geometry_shader_file) {
		std::shared_ptr<Shader> program = std::make_shared<Shader>();
		program->AddShaderFromFile(
			FGLTools::GlSlVertexShader,
			path + "/" + vertex_shader_file,
			{},{path});
		program->AddShaderFromFile(
			FGLTools::GlSlGeometryShader,
			path + "/" +geometry_shader_file,
			{},
			{path});
		program->AddShaderFromFile(
			FGLTools::GlSlFragmentShader,
			path + "/" +fragment_shader_file,
			{},
			{path});
		program->Link();
		return program;
	}




}