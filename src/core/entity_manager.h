
#ifndef CORE_ENTITY_MANAGER_H
#define CORE_ENTITY_MANAGER_H

#include "entity.h"
#include "subsystem.h"
#include <map>

namespace TEngine::Core {

class EntityManager : public SubSystem<EntityManager> {
private:
	inline static unsigned int entityIdCounter = 1;
	std::map<entity, bool> m_toBeDestroyed;
public:
	static std::string typeName() { return "Core::EntityManager"; }

	using SubSystem<EntityManager>::SubSystem;

	void update(float);

	entity create();
	void destroy(entity); // marks entity for destruction. destroyed next frame.
};
}
#endif
