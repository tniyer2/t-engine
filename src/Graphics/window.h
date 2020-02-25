
#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../input/input.h"
#include "camera.h"

using TEngine::InputNS::Input;

namespace TEngine { namespace Graphics {
enum class ScreenMode {
	windowed,
	fullScreen,
	windowedFullScreen
};

class Window {
	private:
		int m_width, m_height;
		const char* m_title;
		ScreenMode m_screenMode = ScreenMode::windowed;

		GLFWwindow* m_windowPtr = NULL;
		bool m_isOpen = false;

		Input& m_input;
		double m_mouseX, m_mouseY;
		bool m_firstMouse = true;

		friend void resize_callback(GLFWwindow*, int, int);
		friend void key_callback(GLFWwindow*, int, int, int, int);
		friend void mouse_callback(GLFWwindow*, double, double);
		friend void scroll_callback(GLFWwindow*, double, double);

		void setViewport();
		void resize();

		void processInput(float);
	public:
		Camera camera;

		Window(int, int, const char*, Camera);
		~Window();

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

void resize_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
void scroll_callback(GLFWwindow*, double, double);
}}
#endif
