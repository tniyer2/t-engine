
#ifndef UTILITY_BOOLEAN_H
#define UTILITY_BOOLEAN_H

#include <cassert>

namespace TEngine::Utility {

template<bool expected>
bool toggle(bool& val) {
	if (val == expected) return false;
	val = expected;
	return true;
}
}
#endif
