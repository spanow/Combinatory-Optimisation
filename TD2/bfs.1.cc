#include "ugraph.h"

#include <deque>
using std::deque;

vector<int> BFS(const UndirectedGraph& graph, int src) {
  const int n = graph.NumNodes();
  vector<int> parent(n, -1);
  parent[src] = src;
  vector<bool> visited(n, false);
  visited[src] = true;
  deque<int> bfs_queue;
  bfs_queue.push_back(src);
  while (!bfs_queue.empty()) {
    const int node = bfs_queue.front();
    bfs_queue.pop_front();
    for (const int neigh : graph.Neighbors(node)) {
      if (!visited[neigh]) {
        visited[neigh] = true;
        parent[neigh] = node;
        bfs_queue.push_back(neigh);
      }
    }
  }
  return parent;
}
