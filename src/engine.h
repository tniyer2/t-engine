
#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "graphics/renderer.h"
#include "graphics/window.h"
#include "core/entity_manager.h"
#include "core/component_manager.h"
#include "core/script_manager.h"
#include "core/resource.h"
#include "core/subsystem.h"
#include "utility/timer.h"

namespace TEngine::Core {

class Engine : public SubSystem<Engine> {
public:
	RootAllocator gRootAllocator;
	ResourceManager gResourceManager;
	ComponentManager gComponentManager;
	ScriptManager gScriptManager;
	EntityManager gEntityManager;
	Graphics::Renderer gRenderer;
private:
	float m_deltaTime = 0;
	Utility::Timer m_timer;
public:
	static std::string typeName() { return "Core::Engine"; }

	Engine() : SubSystem<Engine>() { }

	void startUp();
	void shutDown();
	void runGameLoop();
};
}
#endif
