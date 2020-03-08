
#include "../core/core.h"
#include "window.h"
#include "model.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

using TEngine::Core::entity;
using TEngine::Core::IComponent;
using TEngine::Core::ComponentArray;

#ifndef RENDERER_H
#define RENDERER_H

namespace TEngine { namespace Graphics {

class MeshComponent : public IComponent {
public:
	Model* model;
	MeshComponent() {}
};

class Renderer {
public:
	Renderer() {}

	inline void startUp() {
		if (running) return;
		running = true;
		pStartUp();
	}
	inline void shutDown() {
		if (!running) return;
		running = false;
		pShutDown();
	}
	inline void update(float deltaTime) {
		if (!running) return;
		pUpdate(deltaTime);

	}

	Renderer(const Renderer&) = delete;
	void operator=(const Renderer&) = delete;

	inline Window& getWindow() { return *m_window; }
	inline ComponentArray<MeshComponent>* getMeshComponents() { return m_meshComponents; }
private:
	static bool running;
	Window* m_window = nullptr;
	ComponentArray<MeshComponent>* m_meshComponents = nullptr;

	void pStartUp();
	void pShutDown();
	void pUpdate(float);
};
}}

#endif
