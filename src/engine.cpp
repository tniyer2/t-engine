
#include "engine.h"

namespace TEngine::Core {

void Engine::startUp() {
	SubSystem<Engine>::startUp();

	gRootAllocator.startUp();
	gResourceManager.startUp();
	gComponentManager.startUp();
	gScriptManager.startUp();
	gEntityManager.startUp();
	gRenderer.startUp();
}

void Engine::shutDown() {
	SubSystem<Engine>::shutDown();

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
		if ((m_deltaTime = m_timer.step((float)glfwGetTime())) == 0) continue;

		gScriptManager.update(m_deltaTime);
		gEntityManager.update(m_deltaTime);
		gRenderer.update(m_deltaTime);
	}
}
}
