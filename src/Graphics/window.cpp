
#include "window.h"
#include <fstream>

namespace TEngine { namespace Graphics {

Window::Window(int width, int height, const char* title)
	: m_width(width), m_height(height), m_title(title) {

	m_windowPtr = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
	if (m_windowPtr == NULL) {
		fprintf(stderr, "Could not create window.");
		return;
	}

	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwSetWindowSizeCallback(m_windowPtr, onWindowResize);
	glfwMakeContextCurrent(m_windowPtr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		fprintf(stderr, "Could not load glad.\n");
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
	default:
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

void Window::update() {
	if (m_isOpen) {
		glfwSwapBuffers(m_windowPtr);
		glfwPollEvents();
	}
}

void Window::close() {
	glfwSetWindowShouldClose(m_windowPtr, GLFW_TRUE);
}

Window::~Window() {
	glfwDestroyWindow(m_windowPtr);
}

void onWindowResize(GLFWwindow* windowPtr, int width, int height) {
	Window* window = (Window*)glfwGetWindowUserPointer(windowPtr);
	window->m_width = width;
	window->m_height = height;
	window->setViewport();
}

}}
