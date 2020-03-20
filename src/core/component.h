
#ifndef CORE_ICOMPONENT_H
#define CORE_ICOMPONENT_H

namespace TEngine::Core {

class IComponent {
public:
	IComponent() { }
	IComponent(const IComponent&) = delete;
	void operator=(const IComponent&) = delete;
};
}
#endif
