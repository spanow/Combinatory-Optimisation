#include "main_utils.h"
#include "ugraph.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

inline UndirectedGraph CompleteGraph(int N) {
  UndirectedGraph g(N);
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      g.AddEdge(i, j);
    }
  }
  return g;
}

// Returns the anti-graph of a graph g, which contains the same number
// of nodes, and all the edges that are *not* in g.
// If include_loops is true, also inject the loops (that aren't in g).
//
// Complexity: O(input size + output size).
inline UndirectedGraph AntiGraph(const UndirectedGraph& g, bool include_loops) {
  const int N = g.NumNodes();
  UndirectedGraph anti(N);
  vector<bool> tmp(N, false);
  for (int i = 0; i < N; ++i) {
    for (int j : g.Neighbors(i)) tmp[j] = true;
    if (!include_loops) tmp[i] = true;
    for (int j = i; j < N; ++j) {
      if (tmp[j]) {
        tmp[j] = false;
      } else {
        anti.AddEdge(i, j);
      }
    }
  }
  return anti;
}

struct pair_hash {
  inline std::size_t operator()(const std::pair<int,int> & v) const {
    return v.first*31 + v.second;
  }
};

// Random graph generator: M edges drawn at random.
// If "force_simple" is true, then multi-edges and loops are forbidden,
// otherwise they are both allowed.
inline UndirectedGraph ErdosRenyi(int N, int M, bool force_simple,
                                  Random* random) {
  const int64 anti_M = (int64(N) * (N - 1)) / 2 - M;
  if (force_simple && anti_M < M) {
    CHECK_GE(anti_M, 0);
    return AntiGraph(ErdosRenyi(N, anti_M, true, random),
                     /*include_loops=*/false);
  }
  UndirectedGraph g(N);
  if (!force_simple) {
    for (int i = 0; i < M; ++i) {
      g.AddEdge(random->Uniform(N), random->Uniform(N));
    }
  } else {  // force_simple = true
    std::unordered_set<pair<int, int>, pair_hash> edges;
    while (edges.size() < M) {
      int a = random->Uniform(N);
      int b = random->Uniform(N);
      if (a == b) continue;
      if (a > b) std::swap(a, b);
      if (edges.insert({a, b}).second) g.AddEdge(a, b);
    }
  }
  return g;
}

// Preferential attachment: graphs with skewed degree distributions (following
// a power-law). Search for "Barabasi Albert" on wikipedia.
// This is an approximation: if M < N, the nodes won't be
// picked uniformly at random (because it tries a bit harder to produce a
// connected graph than the standard version, for larger values of M).
UndirectedGraph BarabasiAlbert(int N, int M, Random* random) {
  UndirectedGraph g(N);
  vector<pair<int, int>> edges = {{random->Uniform(N), random->Uniform(N)}};
  int node = 0;
  while (edges.size() < M) {
    const pair<int, int>& edge = edges[random->Uniform(edges.size())];
    const int a = random->Uniform(2) == 0 ? edge.first : edge.second;
    const int b = node < N ? (node++ % N) : random->Uniform(N);
    edges.push_back({a, b});
    g.AddEdge(a, b);
  }
  return g;
}

// Prints a graph to cout or cerr.
inline ostream& operator<<(ostream& out, const UndirectedGraph& graph) {
  out << "Graph with " << graph.NumNodes() << " nodes and " << graph.NumEdges()
      << " edges (listed below): [" << endl;
  for (int node = 0; node < graph.NumNodes(); ++node) {
    vector<int> neighbors = graph.Neighbors(node);
    sort(neighbors.begin(), neighbors.end());
    for (int neigh : neighbors) {
      if (neigh > node) out << "  " << node << " -- " << neigh << endl;
    }
  }
  return out << "]" << endl;
}
inline ostream& operator<<(ostream& out, const DirectedGraph& graph) {
  out << "Graph with " << graph.NumNodes() << " nodes and " << graph.NumArcs()
      << " arcs (listed below): [" << endl;
  for (int node = 0; node < graph.NumNodes(); ++node) {
    vector<int> neighbors = graph.Neighbors(node);
    sort(neighbors.begin(), neighbors.end());
    for (int neigh : neighbors) {
      out << "  " << node << " -> " << neigh << endl;
    }
  }
  return out << "]" << endl;
}

// Whether graph is connected or not.
inline bool IsConnected(const UndirectedGraph& g) {
  const int n = g.NumNodes();
  if (n <= 1) return true;
  vector<bool> visited(n, false);
  vector<int> q = {0};
  visited[0] = true;
  int num = 1;
  while (!q.empty()) {
    const int node = q.back();
    q.pop_back();
    for (int neigh : g.Neighbors(node)) if (!visited[neigh]) {
      visited[neigh] = true;
      q.push_back(neigh);
      ++num;
    }
  }
  return num == n;
}

inline UndirectedGraph Subgraph(const UndirectedGraph& g,
                                const vector<int>& nodes_to_keep) {
  const int n = g.NumNodes();
  vector<int> new_index(n, -1);
  for (int i = 0; i < nodes_to_keep.size(); ++i) {
    new_index[nodes_to_keep[i]] = i;
  }
  UndirectedGraph subgraph(nodes_to_keep.size());
  for (int node : nodes_to_keep) {
    const int new_node = new_index[node];
    for (int neigh : g.Neighbors(node)) if (neigh >= node) {
      const int new_neigh = new_index[neigh];
      if (new_neigh >= 0) subgraph.AddEdge(new_node, new_neigh);
    }
  }
  return subgraph;
}

inline UndirectedGraph LargestComponent(const UndirectedGraph& g) {
  const int n = g.NumNodes();
  vector<bool> visited(n, false);
  vector<int> largest_comp;
  for (int root = 0; root < n; ++root) if (!visited[root]) {
    visited[root] = true;
    vector<int> comp = {root};
    for (int num = 0; num < comp.size(); ++num) {
      const int node = comp[num];
      for (int neigh : g.Neighbors(node)) if (!visited[neigh]) {
        visited[neigh] = true;
        comp.push_back(neigh);
      }
    }
    if (comp.size() > largest_comp.size()) largest_comp.swap(comp);
  }
  return Subgraph(g, largest_comp);
}

// Converts an undirected graph to a directed graph: each edge A--B is
// converted to either:
// - The two arcs A->B and B->A, with probability p_keep_rev_arc
// - or only one of these two arcs (picked uniformly at random).
inline DirectedGraph ToDirected(const UndirectedGraph& ugraph,
                                double p_keep_rev_arc) {
  const int n = ugraph.NumNodes();
  DirectedGraph dgraph(n);
  for (int a = 0; a < n; ++a) {
    for (int b : ugraph.Neighbors(a)) {
      if (b < a) continue;
      if (random() < p_keep_rev_arc * RAND_MAX) {
        dgraph.AddArc(a, b);
        dgraph.AddArc(b, a);
      } else if (random() % 2 == 0) {
        dgraph.AddArc(a, b);
      } else {
        dgraph.AddArc(b, a);
      }
    }
  }
  return dgraph;
}

