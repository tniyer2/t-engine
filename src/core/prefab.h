
#ifndef CORE_PREFAB_H
#define CORE_PREFAB_H

#include "component.h"
#include "entity.h"
#include "id.h"
#include <vector>

namespace TEngine::Core {

struct prefab_tag {};
using prefab = Id<prefab_tag, unsigned int, 0>;

class PrefabNode {
	friend class Prefab;
private:
	std::vector<IComponent*> m_components;

	PrefabNode(entity id) : entityId(id) { }

	entity instantiate(entity);
public:
	const entity entityId;

	void addComponent(IComponent*);
};

class Prefab {
private:
	std::vector<PrefabNode*> m_nodes;
public:
	PrefabNode* createNode();
	entity instantiate();
};
}
#endif
