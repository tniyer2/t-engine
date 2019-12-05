
#include <stdexcept>
#include <vector>

namespace tf {
	using std::vector;

	class Node {
	private:
		vector<Node*> _parents{};
		vector<Node*> _children{};
	public:
		void precede(Node*);
		void succeed(Node*);
		void removeChild(Node*);
		void removeParent(Node*);

		bool hasChild(Node*);
		bool hasChild(Node* n) const {
			return const_cast<const Node*>(this)->hasChild(n);
		}
		bool hasParent(Node*);
		bool hasParent(Node* n) const {
			return const_cast<const Node*>(this)->hasParent(n);
		}

		const vector<Node*>& getChildren() { return _children; }
		const vector<Node*>& getChildren() const { return _children; }
		const vector<Node*>& getParents() { return _parents; }
		const vector<Node*>& getParents() const { return _parents; }
	};

	class WNode : public Node {
	protected:
		float _weight;
	public:
		WNode(float weight=1) : _weight(weight) {}
		float getWeight() { return _weight; }
		float getWeight() const { return _weight; }
	};
}
