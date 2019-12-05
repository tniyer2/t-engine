
#ifndef DCP_H
#define DCP_H
#include "dcp.h"
#endif
#ifndef SERIALIZER_UTILITY_H
#define SERIALIZER_UTILITY_H
#include "serializerUtility.h"
#endif

namespace tf {
	NodeInfo SerializerUtility::_nodeFromId(DCPNode* n, const float factor) {
		int id = n->getId();
		float height = n->getWeight() * factor;
		float y = n->getStartTime() * -99;
		float x = n->getProcessor() * 100;

		StringMap attribs {
			{"pos", "\"" + std::to_string(x) + "," + std::to_string(y) + "!\""},
			{"height", std::to_string(height)},
			{"label", "\"#" + std::to_string(id) + " " + std::to_string(height) + "\""}
		};

		return NodeInfo {
			std::to_string(id),
			attribs
		};
	}

	EdgeInfo SerializerUtility::_edgeFromId(
		DCPNode* from, DCPNode* to, StringMap attribs) {
		return {
			std::to_string(from->getId()),
			std::to_string(to->getId()),
			attribs
		};
	}

	vector<NodeInfo> SerializerUtility::createNodeInfos(
		const vector<DCPNode*>& nodes, const float factor) {

		vector<NodeInfo> node_infos;

		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			DCPNode* n = *it;
			node_infos.push_back(_nodeFromId(n, factor));
		}

		return node_infos;
	}

	vector<EdgeInfo> SerializerUtility::createEdgeInfos(
		const vector<DCPNode*>& nodes,
		const StringMap& strong_edge_attribs,
		const StringMap& weak_edge_attribs) {

		vector<EdgeInfo> edges;

		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			DCPNode* n = *it;
			vector<Node*> children = n->getChildren();

			for (auto c = children.begin(); c != children.end(); ++c) {
				DCPNode* child = (DCPNode*)*c;
				edges.push_back(_edgeFromId(n, child, weak_edge_attribs));
			}
		}

		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			DCPNode* n = *it;
			DCPNode* next = n->getNextInProcessor();

			if (next != NULL) {
				edges.push_back(_edgeFromId(n, next, strong_edge_attribs));
			}
		}

		return edges;
	}
}
