
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <array>
#include <vector>
#include <map>

using std::string;
using std::array;
using std::vector;
using std::map;

namespace TEngine { namespace NS_Input {

enum class KeyState {
	none,
	press,
	repeat,
	release
};

enum class KeyModifiers {
	shift=0x00,
	control=0x02,
	alt=0x04,
	super=0x08,
	capsLock=0x10,
	numLock=0x20
};

struct InputEvent {};

struct KeyEvent : InputEvent {
	unsigned char keycode;
	KeyState state;
	unsigned char mods;
	string character;

	KeyEvent(unsigned char a)
		: keycode(a), state(KeyState::none), mods(0), character("") {}
	KeyEvent(unsigned char a, KeyState b, unsigned char c, string d)
		: keycode(a), state(b), mods(c), character(d) {}
};

struct MouseEvent : InputEvent {
	float posX, posY;
	MouseEvent()
		: posX(0), posY(0) {}
	MouseEvent(float a, float b)
		: posX(a), posY(b) {}
};

struct ScrollEvent : InputEvent {
	float xoffset, yoffset;
	ScrollEvent()
		: xoffset(0), yoffset(0) {}
	ScrollEvent(float a, float b)
		: xoffset(a), yoffset(b) {}
};

KeyState translate_key_action(int action);
map<int, int> get_key_translations();
map<string, int> get_text_translations();

class Input {
public:
	static Input& getInstance() {
		static Input instance;
		return instance;
	}

	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	void clear();
	void pushKeyEvent(int, int, int, int);
	void pushMouseButtonEvent(int, int, int);
	void pushMouseEvent(double, double);
	void pushScrollEvent(double, double);
	KeyEvent pollKey(string);
	bool isKeyDown(string);

	inline MouseEvent getMouseInfo() {
		return m_mouseEvent;
	}
	inline ScrollEvent getScrollInfo() {
		return m_scrollEvent;
	}
private:
	array<char, 98> m_keys;
	vector<KeyEvent> m_keyEvents;
	map<int, int> m_glfwToKey; // Maps GLFW key codes to a contiguous range for convenience.
	map<string, int> m_textToKey; // Maps strings to (not GLFW) key codes.
	MouseEvent m_mouseEvent;
	ScrollEvent m_scrollEvent;

	Input();
};
}}
#endif
