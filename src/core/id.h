
#ifndef ID_H
#define ID_H

namespace TEngine { namespace Core {
	template<class Tag, class T, T default_value>
	class ID {
	private:
		T m_val;
	public:
		// returns ID with default_value
		static ID invalid() { return ID(); }

		ID() : m_val(default_value) { }

		// Explicit constructor
		explicit ID(T val) : m_val(val) { }

		// Explicit conversion to get back the T
		explicit operator T() const { return m_val; }

		friend bool operator==(ID a, ID b) { return a.m_val == b.m_val; }
		friend bool operator!=(ID a, ID b) { return a.m_val != b.m_val; }
	};

	struct entity_tag {};
	typedef ID<entity_tag, unsigned int, 0> entity;
}}
#endif
