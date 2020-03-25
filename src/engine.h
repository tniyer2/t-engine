
#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "graphics/renderer.h"
#include "graphics/window.h"
#include "core/entity_manager.h"
#include "core/component_manager.h"
#include "core/script_manager.h"
#include "core/resource_manager.h"
#include "core/subsystem.h"
#include "utility/timer.h"
#include <string>

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
	Utility::Timer m_timer;
public:
	static std::string typeName() { return "Core::Engine"; }

	void startUp() override;
	void shutDown() override;
	void runGameLoop();
};
}
#endif
