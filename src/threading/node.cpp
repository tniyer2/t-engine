
#ifndef NODE_H
#define NODE_H
#include "node.h"
#endif

namespace tf {
	void Node::precede(Node* n) {
		if (n == NULL) return;

		if (!hasChild(n)) {
			_children.push_back(n);
			n->_parents.push_back(this);
		}
	}

	void Node::succeed(Node* n) {
		if (n == NULL) return;

		if (!hasParent(n)) {
			_parents.push_back(n);
			n->_children.push_back(this);
		}
	}

	void Node::removeChild(Node* n) {
		if (n == NULL) return;

		for (auto i = getChildren().begin(); i != getChildren().end(); ++i) {
			if (*i == n) {
				_children.erase(i);
				return;
			}
		}
	}

	void Node::removeParent(Node* n) {
		if (n == NULL) return;

		for (auto i = getParents().begin(); i != getParents().end(); ++i) {
			if (*i == n) {
				_parents.erase(i);
				return;
			}
		}
	}

	bool Node::hasChild(Node* n) {
		if (n == NULL) return false;

		for (auto i = getChildren().begin(); i != getChildren().end(); ++i) {
			if (*i == n) {
				return true;
			}
		}
		return false;
	}

	bool Node::hasParent(Node* n) {
		if (n == NULL) return false;

		for (auto i = getParents().begin(); i != getParents().end(); ++i) {
			if (*i == n) {
				return true;
			}
		}
		return false;
	}
}
