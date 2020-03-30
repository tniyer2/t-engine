
#ifndef GRAPHICS_MESH_COMPONENT_H
#define GRAPHICS_MESH_COMPONENT_H

#include "mesh.h"
#include "shader.h"
#include "core/resource_handle.h"
#include "core/component.h"

namespace TEngine::Graphics {

class MeshComponent : public Core::TComponent<MeshComponent> {
public:
	Core::ResourceHandle<Mesh> mesh;
	Core::ResourceHandle<Shader> shader;

	using TComponent<MeshComponent>::TComponent;

	void draw(Core::ResourceHandle<Shader>);
};
}
#endif
