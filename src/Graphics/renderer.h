
#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "../core/pooled_component_allocator.h"
#include "../core/pooled_component_array.h"
#include "../core/subsystem.h"
#include "transform_component.h"
#include "transform_component_array.h"
#include "mesh_component.h"
#include "mesh.h"
#include "mesh_id.h"
#include "window.h"
#include <vector>

namespace TEngine::Graphics {

class Renderer : public Core::SubSystem<Renderer> {
	using MeshPool = Core::PooledComponentAllocator<MeshComponent>;
	using MeshArray = Core::PooledComponentArray<MeshComponent>;
	using TransformPool = Core::PooledComponentAllocator<Transform>;

	struct RendererData {
		Window window;
		MeshPool meshAllocator;
		MeshArray meshArray;
		TransformPool transformAllocator;
		TransformComponentArray transformArray;

		RendererData(Core::IRootAllocator& a)
			: meshAllocator(a), meshArray(meshAllocator),
			transformAllocator(a), transformArray(transformAllocator) { }
	};
private:
	RendererData* m_data = nullptr;
	ComponentPtr<Transform> m_root;
public:
	static std::string typeName() { return "Graphics::Renderer"; }
	using SubSystem<Renderer>::SubSystem;

	void startUp();
	void shutDown();
	void update(float);

	Window& getWindow() { checkRunning(); return m_data->window; }
	MeshArray& getMeshArray() { checkRunning(); return m_data->meshArray; }
};
}
#endif
