
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

#include "Graphics/window.h"
#include "Graphics/shader.h"
#include "Graphics/camera.h"
#include "Graphics/model.h"

using namespace TEngine::Graphics;

void printVector(glm::vec4 v) {
	cout << "(" + to_string(v.x) + ", " + to_string(v.y) + ", " + to_string(v.z) + ", " + to_string(v.w) + ")\n";
}

void printMatrix(glm::mat4 matrix) {
	for (unsigned int i = 0; i < 4; i++) {
		printVector(matrix[i]);
	}
	cout << "\n";
}

int main() {
	Window& window = Window::getInstance();

	glClearColor(0, 0.5, 1, 0);
	glEnable(GL_DEPTH_TEST);

	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
	const string& modelPath = "resources/objects/nanosuit/nanosuit.obj";
	Model ourModel(modelPath);

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!window.shouldClose()) {
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		if (deltaTime < 1.0 / 60.0) {
			continue;
		}
		lastFrame = currentFrame;

		window.clear();
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(window.camera.zoom), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
		glm::mat4 view = window.camera.getViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		ourShader.setMat4("model", model);
		ourModel.draw(ourShader);

		window.update(deltaTime);
	}

	return 0;
}
