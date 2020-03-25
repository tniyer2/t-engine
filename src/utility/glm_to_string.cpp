
#include "glm_to_string.h"

#include <sstream>
#include <iomanip>
#include <ios>

namespace TEngine::Utility {

std::string fts(float a) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << a;
	return ss.str();
}

std::string to_string(glm::vec4 v) {
	const static std::string START = "(";
	const static std::string END = ")";
	const static std::string SEP = ", ";

	return START +
		fts(v.x) + SEP +
		fts(v.y) + SEP +
		fts(v.z) + SEP +
		fts(v.w) + END;
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
