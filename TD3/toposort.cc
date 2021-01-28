#include "toposort.h"
#include <deque>
#include <algorithm>
using std::deque;

vector<int> TopologicalSort(const DirectedGraph& graph, int src){
  const int n = graph.NumNodes();
  vector<int> parent(n, -1);
  parent[src] = src;
  vector<bool> visited(n, false);
  visited[src] = true;
  deque<int> topo_queue;
  topo_queue.push_back(src);
  while (!topo_queue.empty()) {
    const int node = topo_queue.front();
    topo_queue.pop_front();
    for (const int neigh : graph.Neighbors(node)) {
      if (!visited[neigh]) {
        visited[neigh] = true;
        parent[neigh] = node;
        topo_queue.push_back(neigh);
      }
    }
  }
  return reverse(parent.begin(), parent.end());
}

tdopt.comb@gmail.com
