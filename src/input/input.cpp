
#include "input.h"
#include <GLFW/glfw3.h>
#include <cassert>

namespace TEngine::InputNS {

std::array<int, 31> KEY_RANGES = {
	32, 39, -1, 44, 57, 59, 61, -1, 65, 93, 96, 161, 162, -1, 256, 269,
	-1, 280, 284, -1, 290, 314, -1, 320, 336, -1, 340, 348,
	-1, 400, 402
};

std::array<std::string, 98> KEY_TEXTS = {
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

std::map<int, int> get_key_translations() {
	std::map<int, int> translations;
	translations[-1] = 0;
	int count = 1;
	int length = KEY_RANGES.size();

	for (unsigned int i = 0; i < length; ++i) {
		int key = KEY_RANGES[i];

		if (key != -1) {
			translations[key] = count++;
		}
		else {
			assert(i < length - 2);

			int start = KEY_RANGES[++i];
			int end = KEY_RANGES[++i];
			assert(start >= 0);
			assert(end >= 0);

			for (int j = start; j <= end; ++j) {
				translations[j] = count++;
			}
		}
	}

	return translations;
}

std::map<std::string, int> get_text_translations() {
	std::map<std::string, int> translations;

	for (unsigned int i = 0; i < KEY_TEXTS.size(); ++i) {
		translations[KEY_TEXTS[i]] = i;
	}

	return translations;
}

KeyState translate_key_action(int action) {
	switch (action) {
	case GLFW_PRESS: return KeyState::press;
	case GLFW_REPEAT: return KeyState::repeat;
	case GLFW_RELEASE: return KeyState::release;
	default: return KeyState::none;
	}
}

Input::Input() {
	m_glfwToKey = get_key_translations();
	m_textToKey = get_text_translations();
	clear();
}

void Input::clear() {
	m_keys.fill(-1);
	m_keyEvents.clear();
	m_mouseEvent = {};
	m_scrollEvent = {};
}

void Input::pushKeyEvent(int glfwKey, int scancode, int action, int mods) {
	int key = m_glfwToKey[glfwKey];
	KeyState state = translate_key_action(action);
	std::string text = KEY_TEXTS[key];
	KeyEvent e(key, state, mods, text);

	m_keyEvents.push_back(e);
	m_keys[key] = m_keyEvents.size() - 1;
}

void Input::pushMouseButtonEvent(int button, int action, int mods) {
	pushKeyEvent(button, 0, action, mods);
}

void Input::pushMouseEvent(double posX, double posY) {
	m_mouseEvent = { (float)posX, (float)posY };
}

void Input::pushScrollEvent(double offsetX, double offsetY) {
	m_scrollEvent = { (float)offsetX, (float)offsetY };
}

KeyEvent Input::pollKey(std::string text) {
	int key = m_textToKey[text];
	if (key != 0) {
		int index = m_keys[key];
		if (index != -1) {
			return m_keyEvents[index];
		}
	}

	return KeyEvent(key, text); // default
}

bool Input::isKeyDown(std::string text) {
	KeyEvent evt = pollKey(text);
	return evt.state == KeyState::press || evt.state == KeyState::repeat;
}
}
