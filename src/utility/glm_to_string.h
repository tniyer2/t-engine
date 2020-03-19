
#ifndef UTILITY_GLM_TO_STRING_H
#define UTILITY_GLM_TO_STRING_H

#include <glm/glm.hpp>
#include <string>

namespace TEngine::Utility {
	using std::string;
	using std::to_string;
	using glm::vec4;
	using glm::mat4;

	string to_string(vec4);
	string to_string(mat4);
}
#endif
