
#include "timer.h"
#include <cassert>

namespace TEngine::Utility {

float Timer::step(float curFrame) {
	float deltaTime = curFrame - m_lastFrame;
	assert(deltaTime >= 0);

	if (deltaTime < m_frameRate) return 0;

	if (deltaTime >= m_maxFrameRate) deltaTime = m_avgFrameRate;

	m_lastFrame = curFrame;
	return deltaTime;
}
}
