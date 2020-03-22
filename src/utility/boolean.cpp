
#include "boolean.h"

namespace TEngine::Utility {

template<bool expected>
bool toggle(bool& val) {
	if (val == expected) return false;
	val = expected;
	return true;
}

template bool toggle<true>(bool&);
template bool toggle<false>(bool&);
}
