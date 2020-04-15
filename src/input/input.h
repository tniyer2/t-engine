
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

struct KeyEvent {
	int keycode = 0;
	KeyState state = KeyState::none;
	int mods = 0;
	std::string character = "";

	KeyEvent(int keycode, std::string character)
		: keycode(keycode), character(character) { }

	KeyEvent(int keycode, KeyState state, int mods, std::string character)
		: keycode(keycode), state(state), mods(mods), character(character) { }
};

struct MouseEvent {
	float posX = 0;
	float posY = 0;

	MouseEvent() { }
	MouseEvent(float x, float y) : posX(x), posY(y) { }
};

struct ScrollEvent {
	float offsetX = 0;
	float offsetY = 0;

	ScrollEvent() { }
	ScrollEvent(float x, float y) : offsetX(x), offsetY(y) { }
};

class Input {
private:
	std::map<int, int> m_glfwToKey; // maps GLFW key codes to contiguous key codes.
	std::map<std::string, int> m_textToKey; // maps strings to contiguous key codes.

	std::array<unsigned int, 98> m_keys; // maps keycode to index in m_keyEvents.
	std::vector<KeyEvent> m_keyEvents; // current KeyEvents
	MouseEvent m_mouseEvent; // current MouseEvent
	ScrollEvent m_scrollEvent; // current ScrollEvent
public:
	Input();
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	void pushKeyEvent(int, int, int, int); // sets current KeyEvent for keycode.
	void pushMouseButtonEvent(int, int, int); // sets current KeyEvent for mouse button.
	void pushMouseEvent(double, double); // sets current MouseEvent.
	void pushScrollEvent(double, double); // sets current ScrollEvent.

	void clear(); // clears all event information.

	KeyEvent pollKey(std::string); // returns KeyEvent for character, default if not found.
	bool isKeyDown(std::string); // returns true if KeyEvent for character.

	MouseEvent pollMouse() { return m_mouseEvent; }
	ScrollEvent pollScroll() { return m_scrollEvent; }
};
}
#endif
