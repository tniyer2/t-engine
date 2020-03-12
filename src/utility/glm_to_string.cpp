
#include "glm_to_string.h"

namespace TEngine { namespace Utility {
	string to_string(vec4 v) {
		return "(" +
			to_string(v.x) + ", " +
			to_string(v.y) + ", " +
			to_string(v.z) + ", " +
			to_string(v.w) + ")";
	}

	string to_string(mat4 m) {
		string s = "[\s";
		for (unsigned int i = 0; i < 4; i++) {
			s += "\s\s" + to_string(m[i]);
			if (i < 3) {
				s += ",\n";
			}
		}
		s += "\s]";
		return s;
	}
}}
