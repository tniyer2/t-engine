
#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "transform_component_array.h"
#include "transform.h"
#include "mesh_component.h"
#include "window.h"
#include "core/pooled_component_array.h"
#include "core/pooled_component_allocator.h"
#include "core/subsystem.h"
#include <string>

namespace TEngine::Graphics {

class Renderer : public Core::SubSystem<Renderer> {
	struct RendererData {
		Window window;
		Core::PooledComponentAllocator<MeshComponent> meshAllocator;
		Core::PooledComponentArray<MeshComponent> meshArray;
		Core::PooledComponentAllocator<Transform> transformAllocator;
		TransformComponentArray transformArray;

		RendererData(Core::IRootAllocator& a)
			: meshAllocator(a), meshArray(meshAllocator),
			transformAllocator(a), transformArray(transformAllocator) { }
	};
private:
	RendererData* m_data = nullptr;
public:
	static std::string typeName() { return "Graphics::Renderer"; }

	using SubSystem<Renderer>::SubSystem;

	Window& getWindow() {
		checkRunning();
		return m_data->window;
	}

	void startUp() override;
	void shutDown() override;
	void update(float);
};
}
#endif
