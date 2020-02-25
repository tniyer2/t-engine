
#include "window.h"

using TEngine::InputNS::KeyState;

namespace TEngine { namespace Graphics {
Window::Window(int width, int height, const char* title, Camera cam)
	: m_width(width), m_height(height),
	m_mouseX(width/2), m_mouseY(height/2),
	m_title(title), m_input(Input::getInstance()), camera(cam) {

	m_windowPtr = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_windowPtr == NULL) {
		std::cout << "Could not create window.";
		return;
	}

	// Set callbacks and Activate
	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwSetWindowSizeCallback(m_windowPtr, resize_callback);
	glfwSetKeyCallback(m_windowPtr, key_callback);
	glfwSetCursorPosCallback(m_windowPtr, mouse_callback);
	glfwSetScrollCallback(m_windowPtr, scroll_callback);
	glfwMakeContextCurrent(m_windowPtr);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not load glad.\n";
		return;
	}

	this->setViewport();
	m_isOpen = true;
}

void Window::setViewport() {
	glViewport(0, 0, m_width, m_height);
}

void Window::resize() {
	if (m_isOpen) {
		glfwSetWindowSize(m_windowPtr, m_width, m_height);
	}
}

void Window::setWidth(int width) {
	m_width = width;
	this->resize();
}

void Window::setHeight(int height) {
	m_height = height;
	this->resize();
}

void Window::setSize(int width, int height) {
	m_width = width;
	m_height = height;
	this->resize();
}

void Window::setTitle(const char* title) {
	m_title = title;
	glfwSetWindowTitle(m_windowPtr, m_title);
}

void Window::setScreenMode(ScreenMode screenMode) {
	if (m_screenMode == screenMode) return;
	m_screenMode = screenMode;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);

	switch (m_screenMode) {
		case ScreenMode::windowed:
			glfwSetWindowMonitor(m_windowPtr, NULL,
				100, 100, m_width, m_height, vidMode->refreshRate);
			break;
		case ScreenMode::fullScreen:
			glfwSetWindowMonitor(m_windowPtr, monitor,
				0, 0, m_width, m_height, vidMode->refreshRate);
			break;
		case ScreenMode::windowedFullScreen:
			glfwSetWindowMonitor(m_windowPtr, monitor,
				0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
			break;
	}
}

bool Window::shouldClose() {
	if (m_isOpen) {
		return !(m_isOpen = !glfwWindowShouldClose(m_windowPtr));
	} else {
		return true;
	}
}

void Window::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update(float deltaTime) {
	if (m_isOpen) {
		glfwSwapBuffers(m_windowPtr);
		m_input.clear();
		glfwPollEvents();
		processInput(deltaTime);
	}
}

void Window::processInput(float deltaTime) {
	if (m_input.isKeyDown("escape")) {
		glfwSetWindowShouldClose(m_windowPtr, true);
	}

	if (m_input.isKeyDown("w")) {
		camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (m_input.isKeyDown("s")) {
		camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (m_input.isKeyDown("a")) {
		camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (m_input.isKeyDown("d")) {
		camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
}

void Window::close() {
	glfwSetWindowShouldClose(m_windowPtr, GLFW_TRUE);
}

Window::~Window() {
	glfwDestroyWindow(m_windowPtr);
}

void resize_callback(GLFWwindow* windowPtr, int width, int height) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->m_width = width;
	window->m_height = height;
	window->setViewport();
}

void key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->m_input.pushKeyEvent(key, scancode, action, mods);
}

void mouse_callback(GLFWwindow* windowPtr, double xpos, double ypos) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	if (window->m_firstMouse) {
		window->m_mouseX = xpos;
		window->m_mouseY = ypos;
		window->m_firstMouse = false;
	}

	float xoffset = xpos - window->m_mouseX;
	float yoffset = window->m_mouseY - ypos; // reversed since y-coordinates go from bottom to top

	window->m_mouseX = xpos;
	window->m_mouseY = ypos;

	window->camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* windowPtr, double xoffset, double yoffset) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->camera.processMouseScroll(yoffset);
}
}}
