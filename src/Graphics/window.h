
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../input/input.h"
#include "camera.h"

using TEngine::NS_Input::Input;

namespace TEngine { namespace Graphics {
enum class ScreenMode {
	windowed,
	fullScreen,
	windowedFullScreen
};

class Window {
private:
	int m_width = 640, m_height = 480;
	const char* m_title = "TEngine";
	ScreenMode m_screenMode = ScreenMode::windowed;

	GLFWwindow* m_windowPtr = NULL;
	bool m_isOpen = false;

	Input& m_input;

	Window();
	~Window();

	void resize();
	void setViewport(); // @todo refactor, this is camera logic
	void processInput(float); // @todo refactor, this is game logic

	friend void resize_callback(GLFWwindow*, int, int);
	friend void key_callback(GLFWwindow*, int, int, int, int);
	friend void mouse_button_callback(GLFWwindow*, int, int, int);
	friend void mouse_callback(GLFWwindow*, double, double);
	friend void scroll_callback(GLFWwindow*, double, double);
public:
	static Window& getInstance() {
		static Window instance;
		return instance;
	}

	Window(const Window&) = delete;
	void operator=(const Window&) = delete;

	Camera camera;

	inline int getWidth() { return m_width; }
	inline int getHeight() { return m_height; }
	inline const char* getTitle() { return m_title; }
	inline ScreenMode getScreenMode() { return m_screenMode; }
	inline bool isOpen() { return m_isOpen; }

	void setWidth(int);
	void setHeight(int);
	void setSize(int, int);
	void setTitle(const char*);
	void setScreenMode(ScreenMode);

	bool shouldClose();
	void clear();
	void update(float);
	void close();
};

void error_callback(int, const char*);
void resize_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
}}
#endif
