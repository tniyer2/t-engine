
#include "../core/core.h"
#include "window.h"
#include "shader.h"
#include "model.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

using TEngine::Core::Entity;
using TEngine::Core::AbstractComponent;

#ifndef RENDERER_H
#define RENDERER_H

namespace TEngine { namespace Graphics {

class MeshComponent : AbstractComponent {
public:
	Model model;

	MeshComponent(Entity e, string path) : AbstractComponent(e), model(path) {}
};

class RendererData {
public:
	Window window;
	Shader* shader = nullptr;
	vector<MeshComponent> components;
};

class MeshComponentView {
public:
	MeshComponentView(RendererData& data) : m_data(data) {}

	MeshComponent& getComponent(Entity e) {
		return m_data.components[e.id];
	}

	MeshComponent& createComponent(Entity e) {
		MeshComponent comp(e, "resources/objects/nanosuit/nanosuit.obj");
		m_data.components.push_back(comp);
		return comp;
	}
private:
	RendererData& m_data;
};

class Renderer {
public:
	Renderer() : m_view(*m_data) {}
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
	inline MeshComponentView& getView() { return m_view; }
private:
	static bool running;
	RendererData* m_data = nullptr;
	MeshComponentView m_view;

	void pStartUp();
	void pShutDown();
	void pUpdate(float);
};
}}

#endif
