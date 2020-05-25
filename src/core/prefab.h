
#ifndef CORE_PREFAB_H
#define CORE_PREFAB_H

#include "graphics/transform.h"
#include "component_handle.h"
#include "typed_component_interface.h"
#include "entity.h"
#include <vector>
#include <glm/glm.hpp>

namespace TEngine::Core {

using Graphics::Transform;

struct PrefabNode {
	PrefabNode* parent = nullptr;
	std::vector<PrefabNode*> children;
	std::vector<ComponentInterface*> components;
	glm::mat4 matrix;

	entity instantiate(ComponentHandle<Transform>);

	~PrefabNode() {
		for (auto it = children.begin(); it != children.end(); ++it) {
			delete (*it);
		}

		for (auto it = components.begin(); it != components.end(); ++it) {
			delete (*it);
		}
	}
};

class Prefab {
private:
	PrefabNode* m_rootNode;
public:
	Prefab(PrefabNode* node) : m_rootNode(node) {
		if (!node) throw "Invalid Argument. Root node must not be null.";
	}
	~Prefab() { delete m_rootNode; }
	entity instantiate();
};
}
#endif
