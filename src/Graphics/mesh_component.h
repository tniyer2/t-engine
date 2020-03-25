
#ifndef GRAPHICS_MESH_COMPONENT_H
#define GRAPHICS_MESH_COMPONENT_H

#include "mesh_id.h"
#include "core/entity.h"
#include "core/read_only_property.h"

namespace TEngine::Graphics {

struct MeshComponent {
	Core::ReadOnlyProperty<entity> entityId;
	mesh meshId;
	unsigned int VAO = 0;

	MeshComponent() { }
	MeshComponent(entity id) : entityId(id) { }
};
}
#endif
