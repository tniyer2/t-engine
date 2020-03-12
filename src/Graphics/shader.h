
#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "../core/id.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace TEngine { namespace Graphics {

struct shader_tag{};
using shaderId = TEngine::Core::Id<shader_tag, unsigned int, 0>;

class Shader {
public:
	shaderId Id;

	Shader(const char*, const char*, const char* =nullptr);

	inline void use() {
		glUseProgram((unsigned int)Id);
	}
	inline void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation((unsigned int)Id, name.c_str()), (int)value);
	}
	inline void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation((unsigned int)Id, name.c_str()), value);
	}
	inline void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation((unsigned int)Id, name.c_str()), value);
	}
	inline void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation((unsigned int)Id, name.c_str()), 1, &value[0]);
	}
	inline void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation((unsigned int)Id, name.c_str()), x, y);
	}
	inline void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation((unsigned int)Id, name.c_str()), 1, &value[0]);
	}
	inline void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation((unsigned int)Id, name.c_str()), x, y, z);
	}
	inline void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation((unsigned int)Id, name.c_str()), 1, &value[0]);
	}
	inline void setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation((unsigned int)Id, name.c_str()), x, y, z, w);
	}
	inline void setMat2(const std::string& name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation((unsigned int)Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	inline void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation((unsigned int)Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	inline void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation((unsigned int)Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// utility function for checking shader compilation/linking errors.
	void checkCompileErrors(GLuint, std::string);
};
}}
#endif
