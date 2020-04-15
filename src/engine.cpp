
#include "engine.h"

namespace TEngine::Core {

void Engine::startUp() {
	SubSystem<Engine>::toggleStartUp();

	gRootAllocator.startUp();
	gResourceManager.startUp();
	gComponentManager.startUp();
	gScriptManager.startUp();
	gEntityManager.startUp();
	gRenderer.startUp();
}

void Engine::shutDown() {
	SubSystem<Engine>::toggleShutDown();

	gRenderer.shutDown();
	gEntityManager.shutDown();
	gScriptManager.shutDown();
	gComponentManager.shutDown();
	gResourceManager.shutDown();
	gRootAllocator.shutDown();
}

void Engine::runGameLoop() {
	checkRunning();

	while (!gRenderer.getWindow().shouldClose()) {
		float deltaTime = m_timer.step((float)glfwGetTime());
		if (deltaTime == 0) continue;

		gScriptManager.update(deltaTime);
		gEntityManager.update(deltaTime);
		gRenderer.update(deltaTime);
	}
}
}
