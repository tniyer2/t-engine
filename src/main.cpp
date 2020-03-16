
#include "core/core.h"
#include "core/script.h"
#include "core/resource.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "utility/timer.h"
#include <windows.h>

using namespace TEngine;
using Core::entity;
using Graphics::MeshComponent;

class Player : public Core::Script {
private:
	Core::ComponentPtr<MeshComponent> m_mesh;
	float m_time = 0;
	float m_time2 = 0;
public:
	using Core::Script::Script;

	void Awake() override {
		auto& compM = Core::ComponentManager::getInstance();
		m_mesh = compM.getComponent<MeshComponent>(entity);
	}

	void Update(float deltaTime) override {
		m_time += deltaTime;
		m_time2 += deltaTime;

		// std::cout << "delta time from player: " << deltaTime << "\n";
		if (m_time > 3.0) {
			m_time = 0;
			std::cout << "delta time from player: " << deltaTime << "\n";
			// std::cout << "mesh id: " << (unsigned int)m_mesh->mesh << "\n";
		}
		if (m_time2 > 10.0) {
			m_time2 = 0;
			// Sleep(3000);
		}
	}
};

void createMeshComponent(unsigned int e) {
	auto ptr = Core::ComponentManager::getInstance().addComponent<MeshComponent>(entity(e));
	ptr->mesh = Graphics::meshId(e);
}

void removeMeshComponent(unsigned int e) {
	Core::ComponentManager::getInstance().removeComponent<MeshComponent>(entity(e));
}

int main() {
	Core::RootAllocator gRootAllocator;
	gRootAllocator.startUp();

	Core::ResourceManager gResourceManager;
	gResourceManager.startUp();

	Core::ComponentManager gComponentManager;
	gComponentManager.startUp();

	Core::ScriptManager gScriptManager;
	gScriptManager.startUp();

	Core::EntityManager gEntityManager;
	gEntityManager.startUp();

	Graphics::Renderer gRenderer;
	gRenderer.startUp();

	Graphics::Window& gWindow = gRenderer.getWindow();

	entity e = gEntityManager.create();
	auto m_mesh = gComponentManager.addComponent<MeshComponent>(e);
	m_mesh->mesh = Graphics::meshId(20);
	gScriptManager.addScript<Player>(e);

	createMeshComponent(12);
	createMeshComponent(2);
	removeMeshComponent(12);

	Utility::Timer timer;

	float deltaTime = 0.0f;
	while (!gWindow.shouldClose()) {
		if ((deltaTime = timer.step((float)glfwGetTime())) == 0) continue;

		gRootAllocator.update(deltaTime);
		gComponentManager.update(deltaTime);
		gScriptManager.update(deltaTime);
		gEntityManager.update(deltaTime);
		gRenderer.update(deltaTime);
	}

	gRenderer.shutDown();
	gEntityManager.shutDown();
	gScriptManager.shutDown();
	gComponentManager.shutDown();
	gResourceManager.shutDown();
	gRootAllocator.shutDown();

	return 0;
}

/*
Entity entity = gEntityManager.createEntity();
Model model("resources/objects/nanosuit/nanosuit.obj");
MeshComponent* comp = gComponentManager.setComponent<MeshComponent>(entity);
comp->model = model;
*/
