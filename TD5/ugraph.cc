#include "ugraph.h"
#include <unordered_set>
using namespace std;

UndirectedGraph::UndirectedGraph(int num_nodes) : dg_(num_nodes) {}

void UndirectedGraph::AddEdge(int a, int b) {
  dg_.AddArc(a, b);
  dg_.AddArc(b, a);
}

int UndirectedGraph::NumNodes() const {
  return dg_.NumNodes();
}

int UndirectedGraph::NumEdges() const {
  return dg_.NumArcs() / 2;
}

int UndirectedGraph::Degree(int node) const {
  return dg_.OutDegree(node);
}

const vector<int>& UndirectedGraph::Neighbors(int node) const {
  return dg_.Neighbors(node);
}

// NOTE: This implementation has the right asymptotical complexity
// but unordered_set makes it much slower than it can be: there is
// a simple implementation that is about 5-6x faster using an external
// vector<bool> bitmask on all the nodes, which is 'all false' upon
// starting and ending this function. Ask fabien.viger@... if interested.
vector<int> UndirectedGraph::GetNodesConnectedTo(int node) const {
  // We remember the set of nodes in the connected component with an
  // unordered_set<>, to guarantee a complexity of O(component size).
  unordered_set<int> visited_nodes = {node};
  vector<int> queue = {node};
  while (!queue.empty()) {
    const int node = queue.back();
    queue.pop_back();
    for (const int neighbor : Neighbors(node)) {
      if (visited_nodes.insert(neighbor).second) {  // New node!
        queue.push_back(neighbor);
      }
    }
  }
  // Directly construct the vector<int> from the unordered_set.
  return {visited_nodes.begin(), visited_nodes.end()};
}

vector<vector<int>> UndirectedGraph::ConnectedComponents() const {
  vector<bool> visited(NumNodes(), false);
  vector<vector<int>> components;
  for (int node = 0; node < NumNodes(); ++node) {
    if (visited[node]) continue;
    components.push_back(GetNodesConnectedTo(node));
    for (const int n : components.back()) visited[n] = true;
  }
  return components;
}
