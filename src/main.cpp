
#include "engine.h"
#include "core/core.h"
#include "core/prefab.h"
#include "graphics/model.h"
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
	Core::ComponentHandle<Transform> m_transform;
	float m_time = 0;
public:
	using Core::Script::Script;

	void Awake() override {
		m_transform = Core::get<Transform>(this->entityId);
	}

	void Update(float deltaTime) override {
		m_time += deltaTime;

		
	}
};

auto makeTransform() {
	return create<Transform>(create<entity>());
}

void userLogic() {
	Graphics::Shader::defaultShader = Graphics::Shader::loadShader(
		"shaders/shader.vs", "shaders/shader.fs");

	Graphics::Model model("resources/objects/nanosuit/nanosuit.obj");
	Core::Prefab* prefab = model.loadModel();

	entity id = prefab->instantiate();
	auto transform = Core::get<Transform>(id);
	Core::addScript<Player>(id);

	glm::mat4 matrix;
	matrix = glm::translate(matrix, { 0, -1.75f, 0 });
	matrix = glm::scale(matrix, { 0.2f, 0.2f, 0.2f });
	transform->setLocalMatrix(matrix);
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
