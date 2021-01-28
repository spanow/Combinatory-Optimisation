#include "bfs.1.h"
#include "bfs.2.h"
#include "bfs.3.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

pair<int, int> DiametreLB(const UndirectedGraph& graph) {
  // On utilisera 50% du temps limite (pour garder une bonne marge).
  const double tmax = 1e-6 * (graph.NumNodes() + graph.NumEdges()) + 50e-3;
  const clock_t deadline = clock() + 0.4 * CLOCKS_PER_SEC * tmax;

  int max_d = -1;
  pair<int, int> res = {0, 0};
  const int n = graph.NumNodes();
  do {
    // first BFS.
    int src = random() % n;
    vector<int> distances = GetBfsDistances(BFS(graph, src));
    const int num_bfs = 2 + (random() % 2);
    for (int bfs = 1; bfs < num_bfs; ++bfs) {
      // we take the farthest node.
      vector<int> farthest;
      int dist_max = -1;
      for (int i = 0; i < n; ++i) {
        if (distances[i] > dist_max) {
          farthest = {i};
          dist_max = distances[i];
        } else if (distances[i] == dist_max) {
          farthest.push_back(i);
        }
      }
      //Random src..
      src = farthest[random() % farthest.size()];
      distances = GetBfsDistances(BFS(graph, src));
    }
    int node = 0;
    for (int i = 1; i < n; ++i) {
      if (distances[i] > distances[node]) node = i;
    }
    //Update diamètre..
    if (distances[node] > max_d) {
      max_d = distances[node];
      res = {src, node};
    }
  } while (clock() < deadline);
  return res;
}
