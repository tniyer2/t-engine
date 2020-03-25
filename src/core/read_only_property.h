
#ifndef CORE_READ_ONLY_PROPERTY_H
#define CORE_READ_ONLY_PROPERTY_H

namespace TEngine::Core {

template<class T>
class ReadOnlyProperty {
private:
	constexpr static const char* InvalidAccessError =
		"Invalid access to ReadOnlyProperty. Cannot set new value when current value evaluates to true.";
	constexpr static const char* InvalidArgument =
		"Invalid argument. Value does not evaluate to true.";

	T m_val;
public:
	ReadOnlyProperty() { }
	ReadOnlyProperty(T val) : m_val(val) {
		if (!val) throw InvalidArgument;
	}

	const T& operator= (const T& val) {
		if (m_val) throw InvalidAccessError;
		if (!val) throw InvalidArgument;
		return m_val = val;
	}
	operator const T& () const { return m_val; }
};
}
#endif
