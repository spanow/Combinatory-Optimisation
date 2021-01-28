#include "color.h"
#include "main_utils.h"
#include "graph_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// Renvoie "" si le coloriage est valide, et un message d'erreur
// decrivant le probleme sinon.
string ColoringError(int num_colors, const UndirectedGraph& graph,
                     const vector<int>& colors) {
  const int N = graph.NumNodes();
  if (colors.size() != N) {
    string error = "Le vector<int> des couleurs n'a pas la meme taille (" +
      to_string(colors.size()) + ") que le nombre de noeuds (" +
      to_string(N) + ")";
    if (colors.size() > N) {
      error += ". Vous avez peut-etre oublié de faire clear()?";
    }
    return error;
  }
  for (int i = 0; i < N; ++i) {
    const int c = colors[i];
    if (c < 0 || c >= num_colors) {
      return "La couleur du noeud #" + to_string(i) + " (egale a " +
        to_string(c) + ") n'est pas entre 0 et num_colors-1 (num_colors=" +
        to_string(num_colors) + ")";
    }
  }
  for (int node = 0; node < N; ++node) {
    for (int neigh : graph.Neighbors(node)) {
      if (node != neigh && colors[node] == colors[neigh]) {
        return "Les noeuds " + to_string(node) + " et " + to_string(neigh) +
          " sont voisins, mais ont la meme couleur " + to_string(colors[node]);
      }
    }
  }
  return "";  // Pas de probleme.
}

struct Perf {
  int num_colors;
  double time_seconds;
};

ostream& operator<<(ostream& out, const Perf& perf) {
  return out << "num_colors=" << perf.num_colors << ", time=" << perf.time_seconds << "s";
}

int main() {
  cout << "=================================================================\n";
  cout << "Correctness tests on small examples." << endl;

  Random random(123);

  cout << "Graph with 3 nodes" << endl;
  {
    UndirectedGraph g(3);
    vector<int> colors;
    int num_colors = Color(g, &colors);
    // Avec aucune arete.
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
    // On ajoute l'arete 1-2, puis 2-0, puis 1-0.
    g.AddEdge(1, 2);
    num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
    g.AddEdge(2, 0);
    num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
    g.AddEdge(1, 0);
    num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  cout << "Corner case: with one node." << endl;
  {
    UndirectedGraph g(1);
    vector<int> colors;
    int num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
    // On ajoute une boucle (0--0).
    g.AddEdge(0, 0);
    num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
    // Et encore une.
    g.AddEdge(0, 0);
    num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  cout << "On a small random graph (10 nodes, 30 edges)" << endl;
  {
    UndirectedGraph g = ErdosRenyi(10, 30, /*force_simple=*/false, &random);
    vector<int> colors;
    int num_colors = Color(g, &colors);
    const string error = ColoringError(num_colors, g, colors);
    if (!error.empty()) {
      cerr << "Failure detected: " << error << endl << "On: " << g
           << "FAILED! (see context above): " << error << endl;
      return 1;
    }
    cout << "PASSED at line " << __LINE__ << endl;
  }

  cout << "On a small complete graph (7 nodes, 7*6/2 = 21 edges)" << endl;
  {
    UndirectedGraph g = CompleteGraph(7);
    vector<int> colors;
    int num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  cout << "On a medium complete graph (100 nodes, 100*99/2 edges)" << endl;
  {
    UndirectedGraph g = CompleteGraph(100);
    vector<int> colors;
    int num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    const int N = 1000;
    const int M = 30000;
    cout << "Randomized stress test with a medium-size graph (" << N
         << " nodes, " << M << " edges, skewed degree distribution)." << endl;
    UndirectedGraph g = BarabasiAlbert(N, M, &random);
    vector<int> colors;
    int num_colors = Color(g, &colors);
    CHECK_EQ(ColoringError(num_colors, g, colors), "");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  cout << "=================================================================\n";
  cout << "Performance tests (number of colors, runtime), on a few graphs\n";
  cout << "NOTE: score = 1 per test if you're as good as the reference. Can be up to 4 if you beat it!" << endl;
  cout << "NOTE: your actual scores will change when I run them on my laptop, since the runtime will change." << endl;

  vector<UndirectedGraph> graphs;
  graphs.push_back(ErdosRenyi(1000, 1500, /*force_simple=*/true, &random));   // Sparse: avg degree = 3.
  graphs.push_back(ErdosRenyi(1000, 15000, /*force_simple=*/true, &random));  // Semi-Sparse: avg degree = 30.
  graphs.push_back(ErdosRenyi(100, 2500, /*force_simple=*/true, &random));    // Dense: density ~= 50%
  graphs.push_back(ErdosRenyi(100, 3500, /*force_simple=*/true, &random));    // Very Dense: density ~= 71%
  graphs.push_back(ErdosRenyi(100, 4900, /*force_simple=*/true, &random));    // Mega Dense: 99%
  graphs.push_back(BarabasiAlbert(1000, 2000, &random));   // Sparse: avg degree = 4.
  graphs.push_back(BarabasiAlbert(1000, 20000, &random));  // Semi-sparse: avg degree = 40.
  graphs.push_back(CompleteGraph(200));
  // Larger versions of the above.
  graphs.push_back(ErdosRenyi(100000, 150000, /*force_simple=*/true, &random));
  graphs.push_back(ErdosRenyi(1000, 350000, /*force_simple=*/true, &random));
  graphs.push_back(BarabasiAlbert(100000, 200000, &random));
  graphs.push_back(BarabasiAlbert(10000, 200000, &random));
  graphs.push_back(CompleteGraph(1000));

  const vector<Perf> ref = {
    {4, 0.000851},
    {13, 0.002728},
    {19, 0.000592},
    {27, 0.000536},
    {75, 0.000938},
    {5, 0.000901},
    {14, 0.006201},
    {200, 0.010616},
    {5, 0.145288},
    {190, 0.040004},
    {5, 0.170011},
    {15, 0.057777},
    {1000, 0.099245}};

  double tot = 0;
  const int repeat = 5;
  bool has_bug = false;
  for (int i = 0; i < graphs.size(); ++i) {
    const UndirectedGraph& graph = graphs[i];
    vector<int> colors;
    Perf p;
    clock_t c0 = clock();
    for (int i = 0; i < repeat; ++i) {
      p.num_colors = Color(graph, &colors);
    }
    p.time_seconds = double(clock() - c0) / CLOCKS_PER_SEC / repeat;
    tot += p.time_seconds;
    if (tot > 15) {
      cerr << "WARNING: the results below may be skipped by the automated"
           << " tests because it takes too long to get them!" << endl;
      tot = -9e9;
    }
    // Verify the coloring.
    const string error = ColoringError(p.num_colors, graph, colors);
    if (!error.empty()) {
      cerr << "ERROR: Invalid coloring: " << error;
      has_bug = true;
    }
    
    const int deltaC = p.num_colors - ref[i].num_colors;
    double color_score =
      deltaC < 0
        ? 2.0
        : std::max(0.0, std::max(0.7 - deltaC / ref[i].num_colors,
                                 1.0 - 0.1 * deltaC));
    const double runtimeF = p.time_seconds / ref[i].time_seconds;
    double perf_score =
      runtimeF <= 1.0
        ? std::min(2.0, 1-0.5*log(runtimeF))  // Max score = 2: ~7x faster than ref.
        : runtimeF <= 2.0  // Score = 1 if runtime in [ref, ref*2]
          ? 1.0
          : 1.0 / (1.0 + 0.2 * log(runtimeF) * log(runtimeF));  // 10x slower -> 0.65, 100x slower -> 0.25
    cout << "Graph #" << i << ": SCORE=" << 0.1*round(10*color_score * perf_score) << " with " << p
         << " (Reference: " << ref[i] << ")" << endl;
  }
  if (!has_bug) cout << "PASSED at line " << __LINE__ << endl;
}
