
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>

#include "core/core.h"
#include "core/pool.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "graphics/model.h"

using namespace TEngine::Core;
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

struct A {
	float a, b, c, d;
};

string printA(A a) {
	return
		to_string(a.a) + " " +
		to_string(a.b) + " " +
		to_string(a.c) + " " +
		to_string(a.d);
}

int main() {
	EntityManager gEntityManager;
	ComponentManager gComponentManager;
	Renderer gRenderer;
	gEntityManager.startUp();
	gComponentManager.startUp();
	gRenderer.startUp();

	gComponentManager.registerComponentArray<MeshComponent>(gRenderer.getMeshComponents());

	Window& window = gRenderer.getWindow();

	glClearColor(0, 0.5, 1, 0);
	glEnable(GL_DEPTH_TEST);

	RootAllocator gRootAllocator;
	PoolAllocator<A> pool(gRootAllocator);

	double startReserve = glfwGetTime();
	index_t poolSize = 100;
	assert(pool.reserve(poolSize));

	double finishReserve = glfwGetTime();

	for (unsigned int i = 0; i < poolSize-1; ++i) {
		entity e = entity(i + 1);
		pool.allocate(e);
		A* ptr = pool.get(e);
		assert(ptr);
		ptr = new (ptr) A{ (float)i, 2, 3, 4 };
		cout << (unsigned int)e << ": " << printA(*ptr) << "\n";
	}

	double now = glfwGetTime();
	cout << "time to reserve memory: " << finishReserve - startReserve << "\n";
	cout << "time to create objects: " << now - finishReserve << "\n";

	pool.allocate(entity(101));
	A* a = pool.get(entity(1));
	assert(a);
	cout << "a: " << printA(*a) << "\n";

	pool.freeAll();

	/*
	Entity entity = gEntityManager.createEntity();
	Model model("resources/objects/nanosuit/nanosuit.obj");
	MeshComponent* comp = gComponentManager.setComponent<MeshComponent>(entity);
	comp->model = model;
	*/

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
		gRenderer.update(deltaTime);
		window.update(deltaTime);
	}

	gComponentManager.shutDown();
	gEntityManager.shutDown();

	return 0;
}
