#include "toposort.h"

vector<int> TopologicalSort(const DirectedGraph& graph) {
  const int num_nodes = graph.NumNodes();

  // On pré-calcule les "parents" de chaque noeud X, i.e. les noeuds qui ont
  // un arc sortant allant vers X. Car nous devons pouvoir itérer rapidement
  // sur les "parents" d'un noeud lors du tri topologique. Ça ressemble à la
  // 1ère question de ce TD (représentation "parent" -> représentation
  // "enfants"), mais à l'envers. Comme on a un DAG, in noeud peut avoir
  // plusieurs "parents".
  vector<vector<int>> parents(num_nodes);
  for (int node = 0; node < num_nodes; ++node) {
    for (int enfant : graph.Neighbors(node)) {
      parents[enfant].push_back(node);
    }
  }

  // On initialize les structures de données du tri topologique:
  // - deg_res[i], initialisé a OutDegree(i), sera le "degré résiduel" du
  //   noeud i: il diminuera de 1 à chaque fois qu'on finit de traiter un
  //   enfant de i.
  // - leaf_nodes est la liste des noeuds "feuilles", de degré résiduel
  //   zéro, et qui peuvent donc être les prochains à prendre dans l'ordre
  //   topologique.
  vector<int> deg_res(num_nodes, 0);
  vector<int> leaf_nodes;
  for (int node = 0; node < num_nodes; ++node) {
    deg_res[node] = graph.OutDegree(node);
    if (deg_res[node] == 0) leaf_nodes.push_back(node);
  }

  // Tri topologique.
  vector<int> tri;
  while (!leaf_nodes.empty()) {
    const int node = leaf_nodes.back();
    leaf_nodes.pop_back();
    tri.push_back(node);
    for (const int parent : parents[node]) {
      if (--deg_res[parent] == 0) leaf_nodes.push_back(parent);
    }
  }
  if (tri.size() != num_nodes) tri.clear();  // Un cycle nous a empêché de finir
  return tri;
}
