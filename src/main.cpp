
#include "engine.h"
#include "core/core.h"
#include <iostream>
#include <string>

using namespace TEngine;
using Core::entity;
using Core::create;
using Core::destroy;
using Graphics::MeshComponent;
using Graphics::Transform;

const static std::string ERROR_DIVIDER = std::string(60, '-') + "\n";

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

void userLogic() {
	entity player = create<entity>();
	create<MeshComponent>(player);
	auto playerTransform = create<Transform>(player);
	Core::addScript<Player>(player);
	Core::removeScript(player);

	entity hand = create<entity>();
	auto handTransform = create<Transform>(hand);
	assert(handTransform);
	playerTransform->addChild(handTransform);

	destroy<entity>(hand);
	destroy<Transform>(player);
}

void start() {
	Core::Engine gEngine;
	gEngine.startUp();
	userLogic();
	gEngine.runGameLoop();
	gEngine.shutDown();
}

void printError(std::string message) {
	std::cout << ERROR_DIVIDER << "ERROR: " << message << "\n" << ERROR_DIVIDER;
}

int main() {
	try {
		start();
	}
	catch (std::exception& e) {
		printError(e.what());
		throw;
	}
	catch (const char* e) {
		printError(e);
		throw;
	}
	catch (std::string& e) {
		printError(e);
		throw;
	}
	return 0;
}
