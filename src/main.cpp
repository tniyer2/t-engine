
#include "core/core.h"
#include "graphics/renderer.h"
#include "graphics/window.h"

using namespace TEngine;

double FRAME_RATE = 1.0 / 60.0;

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
