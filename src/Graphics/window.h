
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace TEngine { namespace Graphics {
enum class ScreenMode { windowed, fullScreen, windowedFullScreen };

class Window {
	private:
		int m_width, m_height;
		const char* m_title;
		ScreenMode m_screenMode = ScreenMode::windowed;

		GLFWwindow* m_windowPtr = NULL;
		bool m_isOpen = false;

		friend void onWindowResize(GLFWwindow*, int, int);
		void setViewport();
		void resize();
	public:
		Window(int, int, const char*);
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
		void update();
		void close();
	};

void onWindowResize(GLFWwindow*, int, int);
}}
