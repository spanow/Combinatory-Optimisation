#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numeric>

#include "bfs.1.h"
#include "bfs.2.h"
#include "bfs.3.h"
#include "main_utils.h"

int Farthest(const vector<int>& bfs_parents, int* d) {
  vector<int> distances = GetBfsDistances(bfs_parents);
  vector<int> farthest;
  int maxd = -1;
  for (int i = 0; i < distances.size(); ++i) {
    if (distances[i] > maxd) {
      farthest = {i};
      maxd = distances[i];
    } else if (distances[i] == maxd) {
      farthest.push_back(i);
    }
  }
  *d = maxd;
  return farthest[random() % farthest.size()];
}

void RemoveDuplicates(vector<int>* v) {
  std::sort(v->begin(), v->end());
  v->erase(std::unique(v->begin(), v->end()), v->end());
}

vector<int> DiametreUB(const UndirectedGraph& graph) {
  const clock_t cmax = clock() + CLOCKS_PER_SEC * 0.4 * (1e-6 * (graph.NumNodes() + graph.NumEdges()) + 50e-3);
  const int n = graph.NumNodes();
  int max_d = 0;
  int min_d = INT_MAX;
  vector<int> best;
  while (clock() < cmax) {
    int d = 0;
    const int a = Farthest(BFS(graph, random() % n), &d);
    const vector<int> bfs = BFS(graph, a);
    const int b = Farthest(bfs, &d);
    vector<int> path = GetShortestPathFromRootedTree(bfs, b);
    max_d = max(max_d, int(path.size()) - 1);
    if (path.size() % 2 == 1) {
      const int center = path[path.size() / 2];
      int r;
      Farthest(BFS(graph, center), &r);
      if (2 * r < min_d) {
        min_d = 2 * r;
        best = {center};
      }
    } else {
      const int c1 = path[path.size() / 2];
      const int c0 = path[path.size() / 2 - 1];
      const vector<int> d0 = GetBfsDistances(BFS(graph, c0));
      const vector<int> d1 = GetBfsDistances(BFS(graph, c1));
      int rmax = 0;
      for (int i = 0; i < n; ++i) {
        int r = std::min(d0[i], d1[i]);
        if (r > rmax) rmax = r;
      }
      if (2 * rmax + 1 < min_d) {
        min_d = 2 * rmax + 1;
        best = {c1, c0};
      }
    }
  }
  if (best.size() >= 2) {
    // Iteratively grow a maximal subclique.
    vector<bool> tmp(n, false);
    vector<int> neigh = graph.Neighbors(best[0]);
    RemoveDuplicates(&neigh);
    for (int i : graph.Neighbors(best[1])) tmp[i] = true;
    tmp[best[0]] = false;
    tmp[best[1]] = false;
    neigh.erase(std::remove_if(neigh.begin(), neigh.end(), [&tmp](int i) { return !tmp[i]; }),
        neigh.end());
    for (int i : graph.Neighbors(best[1])) tmp[i] = false;
    while (!neigh.empty()) {
      const int node = neigh.back();
      neigh.pop_back();
      best.push_back(node);
      for (int i : graph.Neighbors(node)) tmp[i] = true;
      neigh.erase(std::remove_if(neigh.begin(), neigh.end(), [&tmp](int i) { return !tmp[i]; }),
          neigh.end());
      for (int i : graph.Neighbors(node)) tmp[i] = false;
    }
  }
  return best;
}
