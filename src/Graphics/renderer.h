
#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "../core/core.h"
#include "../core/pool.h"
#include "window.h"
#include "mesh.h"

#include <vector>

namespace TEngine::Graphics {

struct MeshComponent {
	meshId mesh;
};

using MeshPool = Core::ComponentAllocator<MeshComponent>;
using MeshArray = Core::PooledComponentArray<MeshComponent>;

struct RendererData {
	Window window;
	MeshPool allocator;
	MeshArray meshArray;

	RendererData(Core::IRootAllocator& root) : allocator(root), meshArray(allocator) { }
};

class Renderer {
private:
	inline static Renderer* instance = nullptr;
	inline static bool running = false;

	RendererData* m_data = nullptr;
public:
	static Renderer& getInstance() {
		assert(instance);
		return *instance;
	}

	Renderer() {
		assert(!instance);
		instance = this;
	}
	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;

	void startUp();
	void shutDown();
	void update(float);

	Window& getWindow() { return m_data->window; }
	MeshArray& getMeshArray() { return m_data->meshArray; }
};
}
#endif
