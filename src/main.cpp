
/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
*/

#include "threading/serializerUtility.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

/*
void error_callback(int error, const char* description) {
	fprintf(stderr, "GLFW Error: %s", description);
}

bool doGLFWStuff() {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		glfwTerminate();

		fprintf(stderr, "Could not initialize GLFW.");
		return false;
	}

	return true;
}
*/

using std::vector;
using std::map;
using std::string;

std::unique_ptr<vector<tf::DCPNode*>> makeNodes(
	const vector<float>& weights, const vector<int>& relations) {

	vector<tf::DCPNode*>* nodes = new vector<tf::DCPNode*>();

	for (auto it = weights.begin(); it != weights.end(); ++it) {
		tf::DCPNode* n = new tf::DCPNode(*it);
		nodes->push_back(n);
	}

	auto relations_end = relations.end();
	auto relations_last = (relations.rbegin() + 1).base();
	for (auto it = relations.begin();
		it != relations_end && it != relations_last; ++(++it)) {

		tf::DCPNode* parent = (*nodes)[*it];
		tf::DCPNode* child = (*nodes)[*++it];
		--it;

		parent->precede(child);
	}

	return std::unique_ptr<vector<tf::DCPNode*>>(nodes);
}

inline auto setup1() {
	return makeNodes(
		{ 1, 1, 1, 1, 1, 1 },
		{ 0,1, 2,4, 2,5, 3,4, 3,5 });
}

inline auto setup2() {
	return makeNodes(
		{ 0.75, 1.25, 1, 2 },
		{ 0,2, 1,2 });
}

inline auto setup3() {
	return makeNodes({}, {});
}

void makeAndOpenGraph(const string from_path, const string to_path) {
	string graph_cmd =
		"neato -n -T svg " +
		from_path + " > " +
		to_path + " & " +
		"\"" + to_path + "\"";
	system(graph_cmd.c_str());
}

int main() {
	const string from_path = "graphs/graph.txt";
	const string to_path = "graphs/graph.svg";

	auto nodes_ptr = setup1();
	auto& nodes = *nodes_ptr;

	tf::DCPGraph g(2);
	for (auto i = nodes.begin(); i != nodes.end(); ++i) {
		g.addNode(*i);
	}
	g.run();

	map<string, string> settings = {};

	map<string, string> node_attribs = {
		{"shape", "box"},
		{"style", "\"filled\""},
		{"overlap", "\"true\""},
		{"fillcolor", "\"#00000080\""},
		{"fontname", "Helvetica"},
		{"fontsize", "9"}
	};
	vector<tf::NodeInfo> other_node_infos = {
		{ "node", node_attribs }
	};

	map<string, string> strong_edge_attribs = {};

	map<string, string> weak_edge_attribs = {
		{"constraint", "weak"},
		{"splines", "false"},
		{"color", "\"#00000077\""}
	};

	vector<tf::EdgeInfo> edge_infos = tf::SerializerUtility::createEdgeInfos(
		nodes, strong_edge_attribs, weak_edge_attribs);

	vector<tf::NodeInfo> node_infos = tf::SerializerUtility::createNodeInfos(nodes);

	std::ofstream stream(from_path);
	tf::Serializer gs(stream);
	gs.start();
	gs.writeDrawingAttribs(settings);
	gs.writeNodes(other_node_infos);
	gs.writeNodes(node_infos);
	gs.writeEdges(edge_infos);
	gs.end();
	stream.close();

	makeAndOpenGraph(from_path, to_path);

	return 0;
}
