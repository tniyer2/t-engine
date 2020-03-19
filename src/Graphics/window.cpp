
#include "window.h"

namespace TEngine::Graphics {

using InputNS::KeyState;
using InputNS::MouseEvent;
using InputNS::ScrollEvent;

Window::Window() {
	// Listen for glfw errors
	glfwSetErrorCallback(error_callback);

	// Initialize glfw
	if (!glfwInit()) {
		glfwTerminate();
		std::cout << "Could not initialize GLFW.\n";
		return;
	}

	// Create window
	m_windowPtr = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
	if (m_windowPtr == NULL) {
		std::cout << "Could not create window.\n";
		return;
	}

	// Set callbacks and activate window
	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwSetWindowSizeCallback(m_windowPtr, Callbacks::resize_callback);
	glfwSetKeyCallback(m_windowPtr, Callbacks::key_callback);
	glfwSetMouseButtonCallback(m_windowPtr, Callbacks::mouse_button_callback);
	glfwSetCursorPosCallback(m_windowPtr, Callbacks::mouse_callback);
	glfwSetScrollCallback(m_windowPtr, Callbacks::scroll_callback);
	glfwMakeContextCurrent(m_windowPtr);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not load glad.\n";
		return;
	}

	m_isOpen = true;
}

void Window::resize() {
	glfwSetWindowSize(m_windowPtr, m_width, m_height);
}

void Window::setWidth(int width) {
	if (!m_isOpen) return;

	m_width = width;
	this->resize();
}

void Window::setHeight(int height) {
	if (!m_isOpen) return;

	m_height = height;
	this->resize();
}

void Window::setSize(int width, int height) {
	if (!m_isOpen) return;

	m_width = width;
	m_height = height;
	this->resize();
}

void Window::setTitle(std::string title) {
	if (!m_isOpen) return;

	m_title = title;
	glfwSetWindowTitle(m_windowPtr, m_title.c_str());
}

void Window::setScreenMode(ScreenMode screenMode) {
	if (!m_isOpen) return;
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
			m_width = vidMode->width;
			m_height = vidMode->height;
			glfwSetWindowMonitor(m_windowPtr, monitor,
				0, 0, m_width, m_height, vidMode->refreshRate);
			break;
	}
}

bool Window::shouldClose() {
	if (!m_isOpen) return true;

	bool close = glfwWindowShouldClose(m_windowPtr);
	m_isOpen = !close;
	return close;
}

void Window::update(float deltaTime) {
	if (!m_isOpen) return;

	glfwSwapBuffers(m_windowPtr);
	input.clear();
	glfwPollEvents();
}

void Window::close() {
	glfwSetWindowShouldClose(m_windowPtr, GLFW_TRUE);
}

Window::~Window() {
	glfwDestroyWindow(m_windowPtr);
}

void Callbacks::resize_callback(GLFWwindow* windowPtr, int width, int height) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->m_width = width;
	window->m_height = height;
}

void Callbacks::key_callback(GLFWwindow* windowPtr, int key, int scancode, int action, int mods) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->input.pushKeyEvent(key, scancode, action, mods);
}

void Callbacks::mouse_button_callback(GLFWwindow* windowPtr, int button, int action, int mods) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->input.pushMouseButtonEvent(button, action, mods);
}

void Callbacks::mouse_callback(GLFWwindow* windowPtr, double xpos, double ypos) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->input.pushMouseEvent(xpos, ypos);
}

void Callbacks::scroll_callback(GLFWwindow* windowPtr, double xoffset, double yoffset) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->input.pushScrollEvent(xoffset, yoffset);
}

void error_callback(int error, const char* description) {
	std::cout << "GLFW Error: " + std::string(description) + "\n";
}
}
