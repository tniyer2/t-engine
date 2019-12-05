
#ifndef DCP_H
#define DCP_H
#include "dcp.h"
#endif

namespace tf {

	void DCPGraph::_topoSort2(vector<DCPNode*>* sorted, const vector<Node*>& children) {
		for (auto i = children.begin(); i != children.end(); ++i) {
			DCPNode* n = (DCPNode*)*i;

			if (n->_tempMark) {
				throw new CyclicDAGError("Cyclic dependency in DAG at node: ");
			}
			else if (n->_permMark) {
				continue;
			}

			n->_tempMark = true;
			_topoSort2(sorted, n->getDependents());
			n->_tempMark = false;
			n->_permMark = true;
			sorted->push_back(n);
		}
	}

	std::unique_ptr<vector<DCPNode*>> DCPGraph::_topoSort(const vector<DCPNode*>& nodes) {
		vector<DCPNode*>* sorted = new vector<DCPNode*>();

		vector<Node*> initial;
		for (auto i = nodes.begin(); i != nodes.end(); ++i) {
			DCPNode* n = *i;
			n->_tempMark = false;
			n->_permMark = false;
			if (n->getRequirements().empty()) {
				initial.push_back(n);
			}
		}

		_topoSort2(sorted, initial);

		return std::unique_ptr<vector<DCPNode*>>(sorted);
	}

	void DCPGraph::_setAdjacentPointers() {
		for (auto it_outer = _processors.begin(); it_outer != _processors.end(); ++it_outer) {
			auto processor = *it_outer;

			if (processor.size() <= 1) {
				continue;
			}

			for (auto it = processor.begin(); it != processor.end(); ++it) {
				DCPNode* n = *it;

				if (it == processor.begin()) {
					n->_next = *(++it);
					--it;
				}
				else if (it == --processor.end()) {
					n->_prev = *(--it);
					++it;
				}
				else {
					n->_prev = *(--it);
					n->_next = *(++(++it));
					--it;
				}
			}
		}
	}

	vector<int> DCPGraph::_getAvailableProcessors(const DCPNode& n, const bool on_dcp) {
		set<int> available;

		/*
		if (on_dcp) {
			vector<Node*> neighbors;
			neighbors.insert(neighbors.end(), n.getParents().begin(), n.getParents().end());
			neighbors.insert(neighbors.end(), n.getChildren().begin(), n.getChildren().end());

			for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
				int p = ((DCPNode*)*it)->_processor;
				if (p >= 0) {
					available.emplace(p);
				}
			}
		}
		*/

		for (unsigned i = 0; i < _processors.size(); ++i) {
			available.emplace(i);

			/*
			bool empty = _processors[i].empty();

			
			if (on_dcp) {
				if (empty) {
					available.emplace(i);
					break;
				}
			}
			else {
				if (!empty) {
					available.emplace(i);
				}
			}
			*/
		}

		return vector<int>(available.begin(), available.end());
	}

	void DCPGraph::_calcAESTs(const vector<DCPNode*>& sorted, const float start) {
		for (auto it_outer = sorted.rbegin(); it_outer != sorted.rend(); ++it_outer) {
			DCPNode* n = *it_outer;
			vector<Node*> reqs = n->getRequirements();
			float earliest;

			if (reqs.empty()) {
				earliest = start;
			}
			else {
				earliest = 0;

				for (auto it = reqs.begin(); it != reqs.end(); ++it) {
					DCPNode* req = (DCPNode*)*it;

					float cur = req->_earliest + req->_weight;
					if (cur > earliest) {
						earliest = cur;
					}
				}
			}

			n->_earliest = earliest;
		}
	}

	float DCPGraph::_calcDCPLength(const vector<DCPNode*>& nodes) {
		float dcpl = 0;

		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			DCPNode* n = (DCPNode*)*it;

			if (n->getDependents().empty()) {
				float cur = n->_earliest + n->_weight;
				if (cur > dcpl) {
					dcpl = cur;
				}
			}
		}

		return dcpl;
	}

	void DCPGraph::_calcALSTs(const vector<DCPNode*>& sorted, const float dcpl) {
		for (auto it_outer = sorted.begin(); it_outer != sorted.end(); ++it_outer) {
			DCPNode* n = *it_outer;
			vector<Node*> deps = n->getDependents();
			float latest;

			if (deps.empty()) {
				latest = dcpl - n->_weight;
			}
			else {
				latest = FLT_MAX;

				for (auto it = deps.begin(); it != deps.end(); ++it) {
					DCPNode* dep = (DCPNode*)*it;

					float cur = dep->_latest - n->_weight;
					if (cur < latest) {
						latest = cur;
					}
				}
			}

			n->_latest = latest;
		}
	}

	void DCPGraph::_calcBounds(const float start=0) {
		_setAdjacentPointers();
		// logNodes(_nodes, "after setting pointers:\n");
		std::unique_ptr<vector<DCPNode*>> sorted = _topoSort(_nodes);

		_calcAESTs(*sorted, start);
		// logNodes(_nodes, "after calculating earliest:\n");

		float dcpl = _calcDCPLength(*sorted);
		cout << "dcpl: " << dcpl << "\n";

		_calcALSTs(*sorted, dcpl);
		logNodes(_nodes, "after calculating latest:\n");
	}

	void DCPGraph::_findSlot(
		const DCPNode& n, const int p, const bool can_push,
		list<DCPNode*>::iterator& k, float& earliest) {

		list<DCPNode*>& processor = _processors[p];
		list<DCPNode*>::iterator k_temp;
		float earliest_temp;

		if (processor.empty()) {
			k_temp = processor.begin();
			earliest_temp = n._earliest;
		}
		else if (can_push) {
			k_temp = processor.begin();
			earliest_temp = n._earliest;

			for (auto it = processor.rbegin(); it != processor.rend(); ++it) {
				DCPNode* j = *it;
				float cur = j->_earliest + j->_weight;

				if (cur <= n._earliest) {
					k_temp = it.base();
					break;
				}
			}
		}
		else {
			earliest_temp = FLT_MAX;

			for (auto it = processor.begin(); it != processor.end(); ++it) {
				DCPNode* j2 = *it;
				DCPNode* j1;
				if (it == processor.begin()) {
					j1 = NULL;
				}
				else {
					j1 = *(--it);
					++it;
				}

				float lower_bound = std::max(j1 == NULL ? 0 : j1->_earliest + j1->_weight, n._earliest);
				float upper_bound = std::min(j2->_latest, n._latest + n._weight);

				if (upper_bound - lower_bound >= n._weight) {
					k_temp = ++it;
					earliest_temp = lower_bound;
					break;
				}
			}

			if (earliest_temp == FLT_MAX) {
				DCPNode* j = *(--processor.end());
				float lower_bound = std::max(j->_earliest + j->_weight, n._earliest);
				float upper_bound = n.getChildren().empty() ? FLT_MAX : n._latest + n._weight;

				if (upper_bound - lower_bound >= n._weight) {
					k_temp = processor.end();
					earliest_temp = lower_bound;
				}
			}
		}

		k = k_temp;
		earliest = earliest_temp;
	}

	void DCPGraph::_selectProcessor(
		const DCPNode& n, const bool on_dcp,
		int& p, list<DCPNode*>::iterator& k, float& earliest) {

		int p_temp = -1;
		list<DCPNode*>::iterator k_temp;
		float earliest_temp = FLT_MAX;
		bool pushed_temp = true;

		vector<int> available = _getAvailableProcessors(n, on_dcp);
		if (available.size() == 0) {
			throw new NoProcessorsError("No processors to place node on.");
		}

		for (auto it = available.begin(); it != available.end(); ++it) {
			int p_loop = *it;
			list<DCPNode*>::iterator k_loop;
			float earliest_loop;
			bool pushed_loop = false;

			_findSlot(n, p_loop, false, k_loop, earliest_loop);
			if (earliest_loop == FLT_MAX /*&& on_dcp*/) {
				_findSlot(n, p_loop, true, k_loop, earliest_loop);
				pushed_loop = true;
			}

			if (earliest_loop < earliest_temp || (earliest_loop == earliest_temp && !pushed_loop && pushed_temp)) {
				p_temp = p_loop;
				k_temp = k_loop;
				earliest_temp = earliest_loop;
				pushed_temp = pushed_loop;
			}
		}

		p = p_temp;
		k = k_temp;
		earliest = earliest_temp;
	}

	void DCPGraph::_nextToSchedule(
		const vector<DCPNode*>& nodes, const list<int>& unscheduled,
		list<int>::const_iterator& it, bool& onDCP) {

		list<int>::const_iterator it_temp;
		float min_diff = FLT_MAX;
		float earliest = FLT_MAX;

		for (auto it_loop = unscheduled.begin(); it_loop != unscheduled.end(); ++it_loop) {
			DCPNode* n = nodes[*it_loop];
			float diff = n->_latest - n->_earliest;

			if (diff < min_diff || (diff == min_diff && n->_earliest < earliest)) {
				it_temp = it_loop;
				min_diff = diff;
				earliest = n->_earliest;
			}
		}

		it = it_temp;
		onDCP = min_diff == 0;
	}

	void DCPGraph::_run() {
		list<int> unscheduled;
		size_t num_nodes = _nodes.size();
		for (unsigned i = 0; i < num_nodes; ++i) {
			unscheduled.push_back(i);
		}

		_calcBounds();

		while (unscheduled.size() != 0) {
			list<int>::iterator index;
			bool on_dcp;
			_nextToSchedule(_nodes, unscheduled, index, on_dcp);
			DCPNode* next = _nodes[*index];
			unscheduled.erase(index);

			cout << "next to schedule: " << next->_id << "\n";

			int p;
			list<DCPNode*>::iterator k;
			float earliest;
			_selectProcessor(*next, on_dcp, p, k, earliest);

			cout << "processor: " << p << "\n";
			cout << "earliest: " << earliest << "\n";

			next->_processor = p;
			next->_earliest = earliest;
			_processors[p].insert(k, next);

			_calcBounds();

			cout << "\n";
		}
	}

	void DCPGraph::addNode(DCPNode* n) {
		if (n == NULL) return;

		n->_id = (int)_nodes.size();
		_nodes.push_back(n);
	}

	void logNodes(const vector<DCPNode*>& nodes, const string s, const bool newline) {
		cout << s;
		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			cout << **it;
		}
		if (newline) {
			cout << "\n";
		}
	}

	std::ostream& operator<<(std::ostream& os, const DCPNode& n) {
		return
			os << "Node " << n._id << " p: " << n._processor <<
			" earliest: " << n._earliest << " latest: " << n._latest <<
			/*
			" prev: " << (n._prev == NULL ? "NULL" : to_string(n._prev->_id)) <<
			" next: " << (n._next == NULL ? "NULL" : to_string(n._next->_id)) <<*/ "\n";
	}
}
