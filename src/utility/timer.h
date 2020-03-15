
#ifndef UTILITY_TIMER_H
#define UTILITY_TIMER_H

namespace TEngine::Utility {

class Timer {
private:
	float m_frameRate;
	float m_avgFrameRate;
	float m_maxFrameRate;
	float m_lastFrame = 0;
public:
	Timer(float frameRate=1.0/60, float maxFrameRate=0.25)
		: m_frameRate(frameRate), m_maxFrameRate(maxFrameRate) {
		m_avgFrameRate = m_frameRate;
	}
	Timer(float frameRate, float maxFrameRate, float avgFrameRate)
		: m_frameRate(frameRate), m_maxFrameRate(maxFrameRate), m_avgFrameRate(avgFrameRate) { }

	/* returns the time passed since the last frame,
	   or 0 if it is less than the fixed frame rate. */
	float step(float);
};
}
#endif
