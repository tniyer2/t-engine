
#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "../core/pooled_component_array.h"
#include "../core/component_pool.h"
#include "../core/subsystem.h"
#include "mesh_component.h"
#include "mesh_id.h"
#include "mesh.h"
#include "window.h"
#include <vector>

namespace TEngine::Graphics {

class Renderer : public Core::SubSystem<Renderer> {
	using MeshPool = Core::PooledComponentAllocator<MeshComponent>;
	using MeshArray = Core::PooledComponentArray<MeshComponent>;

	struct RendererData {
		Window window;
		MeshPool allocator;
		MeshArray meshArray;

		RendererData(Core::IRootAllocator& root)
			: allocator(root), meshArray(allocator) { }
	};
private:
	RendererData* m_data = nullptr;
public:
	using SubSystem<Renderer>::SubSystem;

	void startUp();
	void shutDown();
	void update(float);

	Window& getWindow() { checkRunning(); return m_data->window; }
	MeshArray& getMeshArray() { checkRunning(); return m_data->meshArray; }
};
}
#endif
