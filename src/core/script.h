
#ifndef CORE_SCRIPT_H
#define CORE_SCRIPT_H

#include "entity.h"

namespace TEngine::Core {

class Script {
public:
	const entity entity;
	Script(Core::entity e) : entity(e) { }

	virtual void Awake() { };
	virtual void Update(float) { };
};
}
#endif
