
#ifndef UTILITY_TIMER_H
#define UTILITY_TIMER_H

namespace TEngine::Utility {

class Timer {
private:
	constexpr static float FRAME_RATE = 1.0f / 60.0f;
	constexpr static float MAX_FRAME_RATE = 0.25f;

	float m_frameRate = 0;
	float m_maxFrameRate = 0;
	float m_avgFrameRate = 0;
	float m_lastFrame = 0;
public:
	Timer() : Timer(FRAME_RATE, MAX_FRAME_RATE, FRAME_RATE) { }
	Timer(float rate) : Timer(rate, MAX_FRAME_RATE, rate) { }
	Timer(float rate, float max) : Timer(rate, max, rate) { }

	Timer(float rate, float max, float avg)
		: m_frameRate(rate),
		m_maxFrameRate(max),
		m_avgFrameRate(avg) { }

	float step(float); 	// returns delta time, 0 if less than m_frameRate.
};
}
#endif
