
#ifndef DCP_H
#define DCP_H
#include "dcp.h"
#endif
#ifndef SERIALIZER_H
#define SERIALIZER_H
#include "serializer.h"
#endif

#include <vector>
#include <string>

namespace tf {
	using std::vector;

	class SerializerUtility {
	private:
		static NodeInfo _nodeFromId(DCPNode*, float);
		static EdgeInfo _edgeFromId(DCPNode*, DCPNode*, StringMap);
	public:
		static vector<NodeInfo> createNodeInfos(const vector<DCPNode*>&, float=1);
		static vector<EdgeInfo> createEdgeInfos(
			const vector<DCPNode*>&, const StringMap&, const StringMap&);
	};
}
