
#ifndef GRAPHICS_MESH_COMPONENT_H
#define GRAPHICS_MESH_COMPONENT_H

#include "../core/component.h"
#include "../core/entity.h"
#include "mesh_id.h"

namespace TEngine::Graphics {

struct MeshComponent : public Core::IComponent {
	const entity entityId;
	mesh meshId;
	unsigned int VAO = 0;

	MeshComponent(entity e) : entityId(e) { }
};
}
#endif
