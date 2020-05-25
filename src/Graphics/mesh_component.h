
#ifndef GRAPHICS_MESH_COMPONENT_H
#define GRAPHICS_MESH_COMPONENT_H

#include "mesh.h"
#include "shader.h"
#include "core/resource_handle.h"
#include "core/typed_component_interface.h"

namespace TEngine::Graphics {

class MeshComponent : public Core::TypedComponentInterface<MeshComponent> {
public:
	Core::ResourceHandle<Mesh> mesh;
	Core::ResourceHandle<Shader> shader;

	using TypedComponentInterface<MeshComponent>::TypedComponentInterface;

	void draw(Core::ResourceHandle<Shader>);
};
}
#endif
