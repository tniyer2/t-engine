
#ifndef CORE_ID_H
#define CORE_ID_H

namespace TEngine::Core {

template<class Tag, class T, T default_value>
class Id {
private:
	T m_val;
public:
	static Id invalid() { return Id(); }

	Id() : m_val(default_value) { }
	explicit Id(T val) : m_val(val) { }

	explicit operator T() const { return m_val; }

	operator bool() const { return m_val != default_value; }

	friend bool operator<(Id a, Id b) { return a.m_val < b.m_val; }

	friend bool operator==(Id a, Id b) { return a.m_val == b.m_val; }
	friend bool operator!=(Id a, Id b) { return a.m_val != b.m_val; }
};
}
#endif
