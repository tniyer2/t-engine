
#include "../core/core.h"
#include "window.h"
#include "shader.h"
#include "model.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

using TEngine::Core::Entity;
using TEngine::Core::IComponent;
using TEngine::Core::ComponentArray;

#ifndef RENDERER_H
#define RENDERER_H

namespace TEngine { namespace Graphics {

class MeshComponent : public IComponent {
public:
	Model model;
	MeshComponent(Entity e, string path) : IComponent(e), model(path) {}
};

class RendererData {
public:
	Window window;
	Shader* shader = nullptr;
	vector<MeshComponent> components;
};

class Renderer {
public:
	Renderer() {}
	~Renderer() {}

	inline void startUp() {
		if (running) return;
		running = true;
		pStartUp();
	}
	inline void update(float deltaTime) {
		if (!running) return;
		pUpdate(deltaTime);

	}
	inline void shutDown() {
		if (!running) return;
		running = false;
		pShutDown();
	}

	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;

	inline Window& getWindow() { return m_data->window; }
	inline ComponentArray<MeshComponent>* getView() { return m_view; }
private:
	static bool running;
	RendererData* m_data = nullptr;
	ComponentArray<MeshComponent>* m_view;

	void pStartUp();
	void pShutDown();
	void pUpdate(float);
};
}}

#endif
