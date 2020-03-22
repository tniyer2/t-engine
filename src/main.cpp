
#include "engine.h"
#include <iostream>
#include <string>

using namespace TEngine;
using Core::entity;
using Graphics::MeshComponent;
using Graphics::Transform;

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

template<class T>
auto create(entity id) {
	return Core::ComponentManager::getInstance().addComponent<T>(id);
}

template<class T>
void remove(entity id) {
	Core::ComponentManager::getInstance().removeComponent<T>(id);
}

void userLogic() {
	auto& engine = Core::Engine::getInstance();
	auto& entM = engine.gEntityManager;

	entity player = entM.create();
	create<MeshComponent>(player);
	auto playerTransform = create<Transform>(player);
	engine.gScriptManager.addScript<Player>(player);

	entity hand = entM.create();
	auto handTransform = create<Transform>(hand);
	playerTransform->addChild(handTransform);

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
