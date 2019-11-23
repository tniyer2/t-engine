
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error: %s", description);
}

int main() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit() || true) {
		glfwTerminate();

		fprintf(stderr, "Could not initialize GLFW.");
		return 1;
	}

	return 0;
}
