
#include <ostream>
#include <string>
#include <map>
#include <vector>

namespace tf {
	using std::ostream;
	using std::string;
	using std::map;
	using std::vector;

	using StringMap = map<string, string>;

	struct NodeInfo {
		string label;
		StringMap attributes;
	};

	struct EdgeInfo {
		string from;
		string to;
		StringMap attributes;
	};

	class Serializer {
	private:
		ostream& _stream;

		static void _indent(ostream&, int);
		static void _writeBracketAttribs(ostream&, const StringMap&);
		void _writeDrawingAttribs(ostream&, const StringMap&, int);
		void _writeNodes(ostream&, const vector<NodeInfo>&, int);
		void _writeEdges(ostream&, const vector<EdgeInfo>&, int);
	public:
		Serializer(ostream& stream) : _stream(stream) {}

		void writeDrawingAttribs(const StringMap& a, int b=1) { _writeDrawingAttribs(_stream, a, b); }
		void writeNodes(const vector<NodeInfo>& a, int b=1) { _writeNodes(_stream, a, b); }
		void writeEdges(const vector<EdgeInfo>& a, int b=1) { _writeEdges(_stream, a, b); }
		void start();
		void end();
	};
}
