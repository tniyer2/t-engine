
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/window.h"
#include "Graphics/shader.h"
#include "Graphics/camera.h"
#include "Graphics/model.h"

using namespace TEngine::Graphics;

void error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " + string(description) + "\n";
}

int main() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		glfwTerminate();
		std:cerr << "Could not initialize GLFW.\n";
		return 1;
	}

	Window window(640, 480, "A new window");

	glClearColor(0, 0.5, 1, 0);
	glEnable(GL_DEPTH_TEST);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
	const std::string& modelPath = "resources/objects/nanosuit/nanosuit.obj";
	Model ourModel(modelPath);

	while (!window.shouldClose()) {
		window.clear();

		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		window.update();
	}

	return 0;
}
