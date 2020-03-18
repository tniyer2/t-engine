
#ifndef CORE_SUBSYSTEM_H
#define CORE_SUBSYSTEM_H

#include "../utility/boolean.h"
#include <cassert>
#include <stdexcept>
#include <string>

namespace TEngine::Core {

template<class T>
class SubSystem {
private:
	inline static T* s_instance = nullptr;
	inline static bool s_running = false;
public:
	static std::string typeName() { return "[Name Not Found]"; }
	static bool isRunning() { return s_running; }

	static T& getInstance() {
		if (!s_instance) {
			throw std::out_of_range("Instance of subsytem " + T::typeName() + " has not been created.");
		}
		return *s_instance;
	}

	SubSystem() {
		if (s_instance) {
			throw (T::typeName() + " cannot be constructed. Instance already exists.");
		}
		s_instance = (T*)this;
	}
	SubSystem(const SubSystem&) = delete;
	void operator=(const SubSystem&) = delete;

	void checkRunning() {
		if (!T::s_running) {
			throw ("Invalid state. Cannot access subsystem " + T::typeName() + ".");
		}
	}
	void startUp() {
		if (!Utility::toggle<true>(s_running)) {
			throw ("Invalid state. Cannot start subsytem " + T::typeName() + ".");
		}
	}
	void shutDown() {
		if (!Utility::toggle<false>(s_running)) {
			throw ("Invalid state. Cannot shut down subsytem " + T::typeName() + ".");
		}
	}
};
}
#endif
