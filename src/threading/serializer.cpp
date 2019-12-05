
#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "serializer.h"
#endif

namespace tf {
	void Serializer::_indent(ostream& stream, const int tabs) {
		for (unsigned i = 0; i < tabs; ++i) {
			stream << "\t";
		}
	}

	void Serializer::_writeBracketAttribs(ostream& stream, const StringMap& attribs) {
		if (attribs.empty()) return;

		stream << " [";

		for (auto it = attribs.begin(); it != attribs.end(); ++it) {
			stream << it->first << "=" << it->second;
			if (it != --attribs.end()) {
				stream << " ";
			}
		}

		stream << "]";
	}

	void Serializer::_writeDrawingAttribs(ostream& stream, const StringMap& attribs, const int tabs) {
		for (auto it = attribs.begin(); it != attribs.end(); ++it) {
			_indent(stream, tabs);
			stream << it->first << "=" << it->second << ";\n";
		}
	}

	void Serializer::_writeNodes(ostream& stream, const vector<NodeInfo>& nodes, const int tabs) {
		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			auto& node = *it;

			_indent(stream, tabs);
			stream << node.label;

			_writeBracketAttribs(stream, node.attributes);

			stream << ";\n";
		}
	}

	void Serializer::_writeEdges(ostream& stream, const vector<EdgeInfo>& edges, const int tabs) {
		for (auto it = edges.begin(); it != edges.end(); ++it) {
			auto& edge = *it;

			_indent(stream, tabs);
			stream << edge.from << " -> " << edge.to;

			_writeBracketAttribs(stream, edge.attributes);

			stream << ";\n";
		}
	}

	void Serializer::start() {
		_stream << "digraph {\n";
	}

	void Serializer::end() {
		_stream << "}\n";
	}
}
