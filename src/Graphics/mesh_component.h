
#ifndef GRAPHICS_MESH_COMPONENT_H
#define GRAPHICS_MESH_COMPONENT_H

#include "mesh_id.h"
#include "shader_id.h"
#include "core/component.h"
#include "core/entity.h"

namespace TEngine::Graphics {

class MeshComponent : public Core::TComponent<MeshComponent> {
public:
	mesh meshId;
	unsigned int VAO = 0;

	shader shaderId;

	using TComponent<MeshComponent>::TComponent;
};
}
#endif
