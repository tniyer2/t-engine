
#include <GLFW/glfw3.h>
#include "input.h"

namespace TEngine { namespace NS_Input {

array<int, 31> KEY_RANGES = {
	32, 39, -1, 44, 57, 59, 61, -1, 65, 93, 96, 161, 162, -1, 256, 269,
	-1, 280, 284, -1, 290, 314, -1, 320, 336, -1, 340, 348,
	-1, 400, 402 // first three mouse buttons
};

array<string, 98> KEY_TO_TEXT = {
	"unknown", "\s", "'", ",", "-", ".", "/",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	";", "=",
	"a", "b", "c", "d", "e", "f",
	"g", "h", "i", "j", "k", "l",
	"m", "n", "o", "p", "q", "r", "s",
	"t", "u", "v", "w", "y", "x", "z",
	"[", "\\", "]", "`", "world1", "world2",
	"escape", "enter", "tab",
	"backspace", "insert", "delete",
	"right", "left", "down", "up",
	"page_up", "page_down", "home", "end",
	"caps_lock", "scroll_lock", "num_lock",
	"print_screen", "pause",
	"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9",
	"f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17",
	"f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25",
	"mouse_left", "mouse_right", "mouse_middle"
};

KeyState translate_key_action(int action) {
	switch (action) {
		case GLFW_PRESS: return KeyState::press;
		case GLFW_REPEAT: return KeyState::repeat;
		case GLFW_RELEASE: return KeyState::release;
		default: return KeyState::none;
	}
}

map<int, int> get_key_translations() {
	int count = 0;
	map<int, int> translations({{-1, count++}});

	for (unsigned int i = 0; i < KEY_RANGES.size(); ++i) {
		int key = KEY_RANGES[i];
		if (key == -1) {
			if (i < KEY_RANGES.size() - 2) {
				int start = KEY_RANGES[++i];
				int end = KEY_RANGES[++i];
				for (int j = start; j <= end; ++j) {
					translations[j] = count++;
				}
			}
		}
		else {
			translations[key] = count++;
		}
	}

	return translations;
}

map<string, int> get_text_translations() {
	map<string, int> translations({});
	for (unsigned int i = 0; i < KEY_TO_TEXT.size(); ++i) {
		translations[KEY_TO_TEXT[i]] = i;
	}
	return translations;
}

Input::Input() {
	m_glfwToKey = get_key_translations();
	m_textToKey = get_text_translations();
	clear();
}

void Input::clear() {
	m_keys.fill(-1);
	m_keyEvents.clear();
	m_scrollEvent = {};
}

void Input::pushKeyEvent(int key, int scancode, int action, int mods) {
	key = m_glfwToKey[key];
	KeyState state = translate_key_action(action);
	string text = KEY_TO_TEXT[key];
	KeyEvent e(key, state, mods, text);

	m_keyEvents.push_back(e);
	m_keys[key] = m_keyEvents.size() - 1;
}

void Input::pushMouseButtonEvent(int button, int action, int mods) {
	pushKeyEvent(button, 0, action, mods);
}

void Input::pushMouseEvent(double xpos, double ypos) {
	m_mouseEvent = { (float)xpos, (float)ypos };
}

void Input::pushScrollEvent(double xoffset, double yoffset) {
	m_scrollEvent = { (float)xoffset, (float)yoffset };
}

KeyEvent Input::pollKey(string text) {
	int key = m_textToKey[text];
	if (key != 0) {
		char index = m_keys[key];
		if (index != -1) {
			return m_keyEvents[index];
		}
	}
	return KeyEvent(key);
}

bool Input::isKeyDown(string text) {
	KeyEvent e = pollKey(text);
	return e.state == KeyState::press || e.state == KeyState::repeat;
}

}}
