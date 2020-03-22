
#ifndef UTILITY_GLM_TO_STRING_H
#define UTILITY_GLM_TO_STRING_H

#include <glm/glm.hpp>
#include <string>

namespace TEngine::Utility {

	std::string to_string(glm::vec4);
	std::string to_string(glm::mat4);
}
#endif
