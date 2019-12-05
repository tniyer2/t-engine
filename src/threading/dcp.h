
#ifndef NODE_H
#define NODE_H
#include "node.h"
#endif

#include <ostream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>

namespace tf {
	using std::cout;
	using std::vector;
	using std::list;
	using std::set;
	using std::string;

	class DCPNode : public WNode {
		friend class DCPGraph;
		friend std::ostream& operator<<(std::ostream&, const DCPNode&);
	private:
		int _id = -1, _processor = -1;
		float _earliest = 0, _latest = FLT_MAX;
		DCPNode* _prev = NULL;
		DCPNode* _next = NULL;
		bool _permMark = false, _tempMark = false;
	public:
		DCPNode(float weight=1) : WNode(weight) {}

		vector<Node*> getRequirements() const {
			vector<Node*> v(getParents());
			if (_prev != NULL) {
				v.push_back(_prev);
			}
			return v;
		}
		vector<Node*> getRequirements() {
			return const_cast<const DCPNode*>(this)->getRequirements();
		}
		vector<Node*> getDependents() const {
			vector<Node*> v(getChildren());
			if (_next != NULL) {
				v.push_back(_next);
			}
			return v;
		}
		vector<Node*> getDependents() {
			return const_cast<const DCPNode*>(this)->getDependents();
		}

		int getId() { return _id; }
		int getId() const { return _id; }
		int getProcessor() { return _processor; }
		int getProcessor() const { return _processor; }
		float getStartTime() { return _earliest; }
		float getStartTime() const { return _earliest; }
		float getLatestStartTime() { return _latest; }
		float getLatestStartTime() const { return _latest; }
		DCPNode* getPreviousInProcessor() { return _prev; }
		DCPNode* getPreviousInProcessor() const { return _prev; }
		DCPNode* getNextInProcessor() { return _next; }
		DCPNode* getNextInProcessor() const { return _next; }
	};

	void logNodes(const vector<DCPNode*>&, string, bool=true);

	class CyclicDAGError : public std::runtime_error {
	public:
		CyclicDAGError(const char* s) : std::runtime_error(s) {}
	};

	class NoProcessorsError : public std::runtime_error {
	public:
		NoProcessorsError(const char* s) : std::runtime_error(s) {}
	};

	class DCPGraph {
	private:
		int _numProcessors;
		vector<DCPNode*> _nodes;
		vector<list<DCPNode*>> _processors;

		static void _topoSort2(vector<DCPNode*>*, const vector<Node*>&);
		static std::unique_ptr<vector<DCPNode*>> _topoSort(const vector<DCPNode*>&);

		void _setAdjacentPointers();

		void _calcAESTs(const vector<DCPNode*>&, float);
		float _calcDCPLength(const vector<DCPNode*>&);
		void _calcALSTs(const vector<DCPNode*>&, float);
		void _calcBounds(float);

		void _findSlot(const DCPNode&, int, bool, list<DCPNode*>::iterator&, float&);
		vector<int> _getAvailableProcessors(const DCPNode&, bool);
		void _selectProcessor(const DCPNode&, bool, int&, list<DCPNode*>::iterator&, float&);

		void _nextToSchedule(const vector<DCPNode*>&, const list<int>&, list<int>::const_iterator&, bool&);

		void _run();
	public:
		DCPGraph(int np) : _numProcessors(np), _processors(vector<list<DCPNode*>>(np)) {}

		void run() { _run(); }
		void addNode(DCPNode*);

		int getNumProcessors() { return _numProcessors; }
		int getNumProcessors() const { return _numProcessors; }
		vector<DCPNode*> getAllNodes() const { return _nodes; }
	};
}
