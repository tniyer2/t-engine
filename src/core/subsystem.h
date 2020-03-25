
#ifndef CORE_SUBSYSTEM_H
#define CORE_SUBSYSTEM_H

#include "../utility/boolean.h"
#include <stdexcept>

namespace TEngine::Core {

template<class Derived>
class SubSystem {
private:
	inline static Derived* s_instance = nullptr;
	inline static bool s_running = false;
public:
	static bool isRunning() { return s_running; }

	static Derived& getInstance() {
		if (!s_instance) {
			throw std::out_of_range("Invalid Call. No instance of subsystem " + Derived::typeName());
		}
		return *s_instance;
	}

	SubSystem() {
		if (s_instance) {
			throw (Derived::typeName() + " cannot be constructed. Instance already exists.");
		}
		s_instance = (Derived*)(this);
	}
	virtual ~SubSystem() { }

	SubSystem(const SubSystem&) = delete;
	void operator=(const SubSystem&) = delete;

	virtual void startUp() {
		if (!Utility::toggle<true>(s_running)) {
			throw ("Invalid State. Cannot start subsytem " + Derived::typeName());
		}
	}
	virtual void shutDown() {
		if (!Utility::toggle<false>(s_running)) {
			throw ("Invalid State. Cannot shut down subsytem " + Derived::typeName());
		}
	}
protected:
	void checkRunning() const {
		if (!s_running) {
			throw ("Invalid Call. Subsystem " + Derived::typeName() + " is not running.");
		}
	}
};
}
#endif
