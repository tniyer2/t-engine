
#ifndef CORE_ID_H
#define CORE_ID_H

namespace TEngine::Core {

template<class Tag, class T, T default_value>
class Id {
private:
	T val;
public:
	static Id invalid() { return Id(); }

	Id() : val(default_value) { }
	explicit Id(T v) : val(v) { }

	explicit operator T() const { return val; }
	explicit operator bool() const { return val != default_value; }

	Id& operator() (const Id& base) {
		if (val != default_value) {
			if (base.val == default_value) val = default_value;
			else val += base.val - 1;
		}
		return *this;
	}

	friend bool operator< (Id a, Id b) { return a.val < b.val; }
	friend bool operator== (Id a, Id b) { return a.val == b.val; }
	friend bool operator!= (Id a, Id b) { return a.val != b.val; }
};
}
#endif
