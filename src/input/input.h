
#ifndef INPUT_INPUT_H
#define INPUT_INPUT_H

#include <array>
#include <vector>
#include <map>
#include <string>

namespace TEngine::InputNS {

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
	std::string character;

	KeyEvent(unsigned char keycode)
		: keycode(keycode), state(KeyState::none), mods(0), character("") { }
	KeyEvent(unsigned char keycode, KeyState state, unsigned char mods, std::string character)
		: keycode(keycode), state(state), mods(mods), character(character) { }
};

struct MouseEvent : InputEvent {
	float posX, posY;
	MouseEvent()
		: posX(0), posY(0) { }
	MouseEvent(float x, float y)
		: posX(x), posY(y) { }
};

struct ScrollEvent : InputEvent {
	float xoffset, yoffset;
	ScrollEvent()
		: xoffset(0), yoffset(0) { }
	ScrollEvent(float x, float y)
		: xoffset(x), yoffset(y) { }
};

KeyState translate_key_action(int action);
std::map<int, int> get_key_translations();
std::map<std::string, int> get_text_translations();

class Input {
private:
	std::array<char, 98> m_keys;
	std::vector<KeyEvent> m_keyEvents;
	std::map<int, int> m_glfwToKey; // Maps GLFW key codes to a contiguous range for convenience.
	std::map<std::string, int> m_textToKey; // Maps strings to (not GLFW) key codes.
	MouseEvent m_mouseEvent;
	ScrollEvent m_scrollEvent;
public:
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	void clear();

	void pushKeyEvent(int, int, int, int);
	void pushMouseButtonEvent(int, int, int);
	void pushMouseEvent(double, double);
	void pushScrollEvent(double, double);

	KeyEvent pollKey(std::string);
	bool isKeyDown(std::string);

	MouseEvent getMouseInfo() { return m_mouseEvent; }
	ScrollEvent getScrollInfo() { return m_scrollEvent; }
};
}
#endif
