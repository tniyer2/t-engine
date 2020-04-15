
#ifndef CORE_SUBSYSTEM_H
#define CORE_SUBSYSTEM_H

#include "utility/boolean.h"
#include <stdexcept>

namespace TEngine::Core {

template<class Derived>
class SubSystem {
private:
	inline static bool s_constructed = false;

	bool m_started = false;
	bool m_running = false;
protected:
	inline static Derived* instance = nullptr;
public:
	static Derived& getInstance() {
		return *instance;
	}

	SubSystem() {
		if (!Utility::toggle<true>(s_constructed)) {
			throw ("Subsystem " + Derived::typeName() + " cannot be constructed. Already constructed once.");
		}
		instance = (Derived*)this;
	}
	virtual ~SubSystem() { }

	SubSystem(const SubSystem&) = delete;
	void operator=(const SubSystem&) = delete;
protected:
	void toggleStartUp() {
		if (!Utility::toggle<true>(m_started)) {
			throw ("Cannot start subsytem " + Derived::typeName() + ". Already started.");
		}
		if (!Utility::toggle<true>(m_running)) {
			throw ("Invalid State. Subsytem " + Derived::typeName() + " should not be running.");
		}
	}

	void toggleShutDown() {
		if (!Utility::toggle<false>(m_running)) {
			throw ("Cannot shutdown subsytem " + Derived::typeName() + ". Already shutdown.");
		}
	}

	void checkRunning() const {
		if (!m_running) {
			throw ("Invalid Call. Subsystem " + Derived::typeName() + " is not running.");
		}
	}
};
}
#endif
