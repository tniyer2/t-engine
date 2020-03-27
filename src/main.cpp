
#include "engine.h"
#include "core/core.h"
#include "utility/glm_to_string.h"
#include <iostream>
#include <string>
#include <exception>

using namespace TEngine;
using Core::entity;
using Core::create;
using Core::destroy;
using Graphics::MeshComponent;
using Graphics::Transform;

const static std::string ERROR_DIVIDER = std::string(60, '-') + "\n";

class Player : public Core::Script {
private:
	Core::ComponentHandle<MeshComponent> m_mesh;
	float m_time = 0;
	float m_time2 = 0;
public:
	using Core::Script::Script;

	void Awake() override {
		auto& compM = Core::ComponentManager::getInstance();
		m_mesh = compM.getComponent<MeshComponent>(entityId);
		// std::cout << "delta time from player Awake(): " << m_time << "\n";
	}

	void Update(float deltaTime) override {
		m_time += deltaTime;
		m_time2 += deltaTime;

		// std::cout << "delta time from player: " << deltaTime << "\n";
		if (m_time > 3.0) {
			m_time = 0;
			// std::cout << "delta time from player: " << deltaTime << "\n";
			// std::cout << "mesh id: " << (unsigned int)m_mesh->mesh << "\n";
		}
		if (m_time2 > 10.0) {
			m_time2 = 0;
			// Sleep(3000);
		}
	}
};

auto makeTransform() {
	return create<Transform>(create<entity>());
}

void userLogic() {
	auto player = makeTransform();
	Transform::getRoot()->addChild(player);

	auto a = makeTransform();
	auto b = makeTransform();
	auto c = makeTransform();
	auto d = makeTransform();
	auto e = makeTransform();

	player->addChild(a);
	a->addChild(b);
	player->addChild(c);
	c->addChild(d);
	c->addChild(e);

	/*
	c->setLocalMatrix({{1, 2, 3, 0}, {3, 2, 1, 0}, {2, 3, 1, 0}, {0, 0, 0, 0}});
	auto copy = c->instantiate(entity(100));
	auto t = Core::get<Transform>(copy);
	std::cout << "id: " << (unsigned int)(entity)t->entityId << "\n";
	c->setLocalMatrix({});
	std::cout << "c matrix:\n" << Utility::to_string(c->getLocalMatrix()) << "\n";
	std::cout << "t matrix:\n" << Utility::to_string(t->getLocalMatrix()) << "\n";*/
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
