
#include "renderer.h"
#include "../utility/boolean.h"

namespace TEngine::Graphics {

void Renderer::startUp() {
	if (!Utility::toggle<true>(running)) return;

	m_data = new RendererData(Core::RootAllocator::getInstance());

	Core::ComponentManager::getInstance().registerComponentArray<MeshComponent>(
		(Core::IComponentArray<MeshComponent>&)m_data->meshArray);

	glClearColor(0, 0.5, 1, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::shutDown() {
	if (!Utility::toggle<false>(running)) return;

	delete m_data;
}

void Renderer::update(float deltaTime) {
	m_data->window.update(deltaTime);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
