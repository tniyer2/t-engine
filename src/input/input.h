
#ifndef INPUT_H
#define INPUT_H

# include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <vector>
#include <map>

using std::string;
using std::array;
using std::vector;
using std::map;

namespace TEngine { namespace InputNS {

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

struct MouseMoveEvent : InputEvent {
	float posX, posY;
};

struct ScrollEvent : InputEvent {
	float xoffset, yoffset;
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

	Input(Input const&) = delete;
	void operator=(Input const&) = delete;

	void clear();
	void pushKeyEvent(int, int, int, int);
	KeyEvent pollKey(string);
	bool isKeyDown(string);
private:
	array<char, 95> m_keys;
	vector<KeyEvent> m_events;
	map<int, int> m_glfwToKey; // Maps GLFW key codes to a contiguous range for convenience.
	map<string, int> m_textToKey; // Maps strings to (not GLFW) key codes.

	Input();
};
}}
#endif
