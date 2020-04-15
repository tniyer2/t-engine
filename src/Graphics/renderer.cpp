
#include "renderer.h"

#include "depth_first_transform_iterator.h"
#include "core/component_manager.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace TEngine::Graphics {

void Renderer::startUp() {
	SubSystem<Renderer>::toggleStartUp();

	m_data = new RendererData(Core::RootAllocator::getInstance());
	m_data->meshAllocator.reserve(100);
	m_data->transformAllocator.reserve(100);
	m_data->cameraAllocator.reserve(10);

	auto& compM = Core::ComponentManager::getInstance();
	compM.registerComponentArray<MeshComponent>(m_data->meshArray);
	compM.registerComponentArray<Transform>(m_data->transformArray);
	compM.registerComponentArray<Camera>(m_data->cameraArray);

	m_data->transformArray.setRootTransform();

	glClearColor(0, 0.5, 1, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::shutDown() {
	SubSystem<Renderer>::toggleShutDown();
	m_data->cameraAllocator.freeAllPools();
	m_data->transformAllocator.freeAllPools();
	m_data->meshAllocator.freeAllPools();
	delete m_data;
}

void Renderer::update(float deltaTime) {
	checkRunning();

	Window& window = m_data->window;
	float width = (float)window.getWidth();
	float height = (float)window.getHeight();
	
	glm::vec3 position = { 0, 0, 3.0f };
	glm::vec3 front = { 0, 0, -1.0f };
	glm::vec3 up = { 0, 1.0f, 0 };
	glm::mat4 view = glm::lookAt(position, position + front, up);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto it = DepthFirstTransformIterator(Transform::getRoot());
	for (; it; ++it) {
		entity id = it->entityId;
		auto mesh = Core::get<MeshComponent>(id);
		if (!mesh) continue;

		Core::ResourceHandle<Shader> shader;
		(bool)mesh->shader;
		if (mesh->shader) {
			shader = mesh->shader;
		}
		else {
			shader = Shader::defaultShader;
		}

		shader->use();

		glm::mat4 projection = m_camera.getPerspectiveMatrix(width, height);
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		shader->setMat4("model", it->getWorldMatrix());

		mesh->draw(shader);
	}

	window.update(deltaTime);
}
}
