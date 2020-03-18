
#include "engine.h"
#include <iostream>
#include <string>

using namespace TEngine;
using Core::entity;
using Graphics::MeshComponent;

static std::string ERROR_DIVIDER = std::string(60, '-');

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

void userLogic() {
	auto& engine = Core::Engine::getInstance();

	entity e = engine.gEntityManager.create();
	createMeshComponent(e);
	engine.gScriptManager.addScript<Player>(e);

	createMeshComponent(3);
	createMeshComponent(12);
	createMeshComponent(2);
	removeMeshComponent(12);
}

void printError(std::string s) {
	std::cout << ERROR_DIVIDER << "\n\nERROR: " << s << "\n\n" << ERROR_DIVIDER << "\n";
}

int main() {
	try {
		Core::Engine gEngine;
		gEngine.startUp();
		userLogic();
		gEngine.runGameLoop();
		gEngine.shutDown();
	}
	catch (std::exception & e) {
		printError(e.what());
		throw;
	}
	catch (const char* e) {
		printError(e);
		throw;
	}
	catch (std::string e) {
		printError(e);
		throw;
	}
	return 0;
}
