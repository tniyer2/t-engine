
#include "Graphics/window.h"
#include <iostream>
#include <fstream>

using namespace TEngine;

void error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error: %s\n", description);
}

int main() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		glfwTerminate();
		fprintf(stderr, "Could not initialize GLFW.\n");
		return 1;
	}

	Graphics::Window window(640, 480, "A new window");

	glClearColor(0, 0.5, 1, 0);

	while (!window.shouldClose()) {
		window.clear();
		window.update();
	}

	return 0;
}
