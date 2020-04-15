
#ifndef CORE_ENTITY_MANAGER_H
#define CORE_ENTITY_MANAGER_H

#include "subsystem.h"
#include "entity.h"
#include <atomic>
#include <map>

namespace TEngine::Core {

class EntityManager : public SubSystem<EntityManager> {
private:
	inline static std::atomic_int entityIdCounter = std::atomic_int(1);
	std::map<entity, bool> m_toBeDestroyed;
public:
	static std::string typeName() { return "Core::EntityManager"; }

	using SubSystem<EntityManager>::SubSystem;

	void startUp() { SubSystem<EntityManager>::toggleStartUp(); }
	void shutDown() { SubSystem<EntityManager>::toggleShutDown(); }
	void update(float);

	entity create();
	void destroy(entity); // marks entity for destruction. destroyed next frame.
};
}
#endif
