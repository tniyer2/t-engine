
#include "renderer.h"
#include "../core/component_view.h"
#include "../core/component_manager.h"

namespace TEngine::Graphics {

void Renderer::startUp() {
	SubSystem<Renderer>::startUp();

	auto& compM = Core::ComponentManager::getInstance();

	m_data = new RendererData(Core::RootAllocator::getInstance());
	m_data->meshAllocator.reserve(100);
	m_data->transformAllocator.reserve(100);

	compM.registerComponentArray<MeshComponent>(m_data->meshArray);
	compM.registerComponentArray<Transform>(m_data->transformArray);

	m_root = m_data->transformArray.setRoot();

	glClearColor(0, 0.5, 1, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::shutDown() {
	SubSystem<Renderer>::shutDown();
	delete m_data;
}

void Renderer::update(float deltaTime) {
	checkRunning();

	m_data->window.update(deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto view = Core::ComponentView<MeshComponent, Transform>();
	while (view) {
		auto next = view.next();
		if (!next.has_value()) continue;
		auto t = next.value();
		auto m = std::get<ComponentPtr<MeshComponent>>(t);
		std::cout << "entity id: " << (unsigned int)m->entityId << "\n";
	}
}
}

/*
Window& window = m_data->window;
Shader& shader = *m_data->shader;

shader.use();

// view/projection transformations
glm::mat4 projection = window.camera.getPerspectiveMatrix();
glm::mat4 view = window.camera.getViewMatrix();
shader.setMat4("projection", projection);
shader.setMat4("view", view);

glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
shader.setMat4("model", model);

vector<MeshComponent> arr = m_data->components;
for (auto it = arr.begin(); it < arr.end(); ++it) {
	it->model.draw(shader);
}
*/
