
#include "glm_to_string.h"

namespace TEngine::Utility {

std::string to_string(glm::vec4 v) {
	const static std::string START = "(";
	const static std::string END = ")";
	const static std::string SEP = ", ";

	return START +
		std::to_string(v.x) + SEP +
		std::to_string(v.y) + SEP +
		std::to_string(v.z) + SEP +
		std::to_string(v.w) + END;
}

std::string to_string(glm::mat4 m) {
	const static std::string START = "[ ";
	const static std::string END = " ]";
	const static std::string SEP = ",\n  ";

	return START +
		to_string(m[0]) + SEP +
		to_string(m[1]) + SEP +
		to_string(m[2]) + SEP +
		to_string(m[3]) + END;
}
}
