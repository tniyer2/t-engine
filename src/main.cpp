
#include "core/core.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "core/stack.h"

using namespace TEngine;

double FRAME_RATE = 1.0 / 60.0;

struct A {
	float a, b, c, d;
};

size_t ceil(size_t a, size_t b) {
	return (a + b - 1) / b;
}

int main() {
	Core::RootAllocator gRootAllocator;
	gRootAllocator.startUp();

	Core::ComponentManager gComponentManager;
	gComponentManager.startUp();

	Core::EntityManager gEntityManager;
	gEntityManager.startUp();

	Graphics::Renderer gRenderer;
	gRenderer.startUp();

	Graphics::Window& gWindow = gRenderer.getWindow();

	auto m = gComponentManager.addComponent<Graphics::MeshComponent>(Core::entity(1));

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	while (!gWindow.shouldClose()) {
		float curFrame = (float)glfwGetTime();
		deltaTime = curFrame - lastFrame;
		if (deltaTime < FRAME_RATE) {
			continue;
		}
		lastFrame = curFrame;

		gRootAllocator.update(deltaTime);
		gComponentManager.update(deltaTime);
		gEntityManager.update(deltaTime);
		gRenderer.update(deltaTime);
	}

	gRenderer.shutDown();
	gEntityManager.shutDown();
	gComponentManager.shutDown();
	gRootAllocator.shutDown();

	return 0;
}

/*
Entity entity = gEntityManager.createEntity();
Model model("resources/objects/nanosuit/nanosuit.obj");
MeshComponent* comp = gComponentManager.setComponent<MeshComponent>(entity);
comp->model = model;
*/
