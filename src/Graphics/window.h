
#ifndef GRAPHICS_WINDOW_H
#define GRAPHICS_WINDOW_H

#include "../input/input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

namespace TEngine::Graphics {

using TEngine::NS_Input::Input;

void error_callback(int, const char*);

class Callbacks {
public:
	static void resize_callback(GLFWwindow*, int, int);
	static void key_callback(GLFWwindow*, int, int, int, int);
	static void mouse_button_callback(GLFWwindow*, int, int, int);
	static void mouse_callback(GLFWwindow*, double, double);
	static void scroll_callback(GLFWwindow*, double, double);
};

enum class ScreenMode {
	windowed,
	fullScreen,
	windowedFullScreen
};

class Window {
public:
	Input input;
private:
	GLFWwindow* m_windowPtr = nullptr;
	std::string m_title = "TEngine";

	int m_width = 640;
	int m_height = 480;
	ScreenMode m_screenMode = ScreenMode::windowed;
	bool m_isOpen = false;
public:
	friend class Callbacks;

	Window();
	~Window();
	Window(const Window&) = delete;
	void operator=(const Window&) = delete;

	// getters
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	std::string getTitle() { return m_title; }
	ScreenMode getScreenMode() { return m_screenMode; }
	bool isOpen() { return m_isOpen; }

	// setters
	void setWidth(int);
	void setHeight(int);
	void setSize(int, int);
	void setTitle(std::string);
	void setScreenMode(ScreenMode);

	void update(float);
	bool shouldClose();
	void close();
private:
	void resize();
};
}
#endif
