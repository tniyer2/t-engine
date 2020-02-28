
#include "renderer.h"

using glm::mat4;

namespace TEngine { namespace Graphics {

bool Renderer::running = false;

void Renderer::pStartUp() {
	m_data = new RendererData();
	m_data->shader = new Shader("shaders/shader.vs", "shaders/shader.fs");
}

void Renderer::pShutDown() {
	delete m_data->shader;
	delete m_data;
}

void Renderer::pUpdate(float deltaTime) {
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
}
}}
