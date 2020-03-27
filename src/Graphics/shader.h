
#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include "shader_id.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace TEngine::Graphics {

class Shader {
public:
	shader shaderId;

	Shader(const char*, const char*, const char* = nullptr);
	Shader(shader id, const char* v, const char* f, const char* g=nullptr)
		: Shader(v, f, g) { shaderId = id; }

	inline void use() {
		glUseProgram((unsigned int)shaderId);
	}
	inline void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation((unsigned int)shaderId, name.c_str()), (int)value);
	}
	inline void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation((unsigned int)shaderId, name.c_str()), value);
	}
	inline void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation((unsigned int)shaderId, name.c_str()), value);
	}
	inline void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation((unsigned int)shaderId, name.c_str()), 1, &value[0]);
	}
	inline void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation((unsigned int)shaderId, name.c_str()), x, y);
	}
	inline void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation((unsigned int)shaderId, name.c_str()), 1, &value[0]);
	}
	inline void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation((unsigned int)shaderId, name.c_str()), x, y, z);
	}
	inline void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation((unsigned int)shaderId, name.c_str()), 1, &value[0]);
	}
	inline void setVec4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation((unsigned int)shaderId, name.c_str()), x, y, z, w);
	}
	inline void setMat2(const std::string& name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation((unsigned int)shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	inline void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation((unsigned int)shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	inline void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation((unsigned int)shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	void checkCompileErrors(GLuint, std::string); // checks shader compilation/linking errors.
};
}
#endif
