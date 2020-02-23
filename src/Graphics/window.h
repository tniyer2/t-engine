
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"

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

		double m_mouseX = 0, m_mouseY = 0;
		bool m_firstMouse = true;

		friend void resizeCallback(GLFWwindow*, int, int);
		friend void mouseCallback(GLFWwindow*, double, double);
		friend void scrollCallback(GLFWwindow*, double, double);

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

void resizeCallback(GLFWwindow*, int, int);
void mouseCallback(GLFWwindow*, double, double);
void scrollCallback(GLFWwindow*, double, double);
}}
