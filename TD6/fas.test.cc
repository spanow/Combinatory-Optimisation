#include <algorithm>
#include <climits>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "fas.h"
#include "graph.h"
#include "graph_utils.h"
#include "main_utils.h"
#include "toposort.h"
#include "ugraph.h"

using namespace std;

int gcd(int a, int b) {
  if (a < b) std::swap(a, b);
  while (b) {
    int c = a % b;
    a = b;
    b = c;
  }
  return a;
}

UndirectedGraph GcdGraph(int N, int min_gcd) {
  UndirectedGraph g(N);
  for (int i = 1; i < N; ++i) {
    for (int j = 1; j < i; ++j) {
      if (gcd(i, j) >= min_gcd) g.AddEdge(i, j);
    }
  }
  for (int i = 1; i < N; ++i) {
    g.AddEdge(i - 1, i);
  }
  return g;
}

int main() {
  Random random(123);
  cout << "Performance tests on a few graphs\n";
  cout << "NOTE: you'll get 0 overall if there is a bug (segfault, out of bounds answer), even if it only happens in 1 test" << endl;
  cout << "NOTE: trespassing the time limit means 0 score for that test." << endl;

  vector<UndirectedGraph> ugraphs;
  ugraphs.push_back(LargestComponent(ErdosRenyi(1000, 1000, /*force_simple=*/true, &random)));
  ugraphs.push_back(LargestComponent(ErdosRenyi(10000, 15000, /*force_simple=*/true, &random)));
  ugraphs.push_back(LargestComponent(ErdosRenyi(100000, 120000, /*force_simple=*/true, &random)));
  ugraphs.push_back(ErdosRenyi(1000, 1000, /*force_simple=*/false, &random));
  ugraphs.push_back(ErdosRenyi(1000, 20000, /*force_simple=*/false, &random));
  ugraphs.push_back(LargestComponent(GcdGraph(1000, 3)));
  ugraphs.push_back(LargestComponent(GcdGraph(1000, 20)));
  ugraphs.push_back(BarabasiAlbert(1000, 1000, &random));
  ugraphs.push_back(BarabasiAlbert(10000, 15000, &random));
  ugraphs.push_back(BarabasiAlbert(100000, 200000, &random));

  int ref[] = {785,786,834,940,9910,10184,12024,14961,88243,89209,98496,118200,837,840,886,998,10234,11191,15116,19978,60562,66401,90265,119794,8013,8794,11840,15597,999,999,999,999,10688,10965,12552,14998,120818,127233,156931,199999};
  int ref0[] = {782,781,817,939,9800,10099,11960,14950,88171,89015,98154,118000,831,837,872,997,10052,11169,14993,19975,60030,65957,90009,119792,7876,8718,11731,15595,998,998,998,998,10606,10893,12476,14997,120563,126734,156800,199998};
  const double coeff[] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
  double total_score = 0;
  int test = 0;
  for (int i = 0; i < ugraphs.size(); ++i) {
    const UndirectedGraph& ug = ugraphs[i];
    double test_score = 0;
    for (double p : {0.0, 0.1, 0.5, 1.0}) {
      const DirectedGraph dg = ToDirected(ug, p);
      const int n = dg.NumNodes();
      const double max_time = 10e-6 * (n + dg.NumArcs()) + 50e-3;
      std::unordered_map<pair<int, int>, int, pair_hash> arc_count;
      for (int a = 0; a < n; ++a) {
        for (int b : dg.Neighbors(a)) {
          ++arc_count[pair<int, int>{a, b}];
        }
      }
      srandom(123);
      clock_t c0 = clock();
      DirectedGraph res = MaxDAG(dg);
      const double t = double(clock() - c0) / CLOCKS_PER_SEC;
      // On verifie que c'est un sous-graphe.
      for (int a = 0; a < n; ++a) {
        for (int b : res.Neighbors(a)) {
          if (--arc_count[pair<int, int>{a, b}] < 0) {
            cerr << "ERROR: Arc " << a << "->" << b
                 << " was in the subgraph produced by MaxDAG() more times than"
                 << " it is in the input! You didn't produce a subgraph.\n";
            return 1;
          }
        }
      }
      // On verifie que c'est un DAG.
      if (TopologicalSort(res).empty()) {
        cerr << "ERROR: Your graph isn't a DAG! A cycle was detected." << endl;
        if (i == 2) {
          cerr << "NOTE: the input of Tests #2 are graphs wit self-arcs A->A.\n"
               << "The fact that your code starts failing for these tests"
               << "*probably* means that your\ncode does not properly"
               << " remove self-arcs (they should never be in the DAG output)\n";
        }
        return 1;
      }
      const int narcs = res.NumArcs();
      double score = std::min(3.0, pow(0.5, double(ref[test] - narcs) / (ref[test] - ref0[test])));
      if (t > max_time) score = 0;
      printf("TEST #%d/%.1lf : Score=%.1lf : Num arcs = %6d (ref: %6d; 50%%: %6d),"
             " time = %.1lf%% of limit\n",
             i, p, score, narcs, ref[test], ref0[test],
             0.1 * int(1000 * t / max_time));
      if (t > max_time) {
        score = 0;
        cout << "ERROR: Time limit trespassed! This will probably get score zero" << endl;
      } else if (t >= 0.5 * max_time) {
        cout << "WARNING: the time is more than 50% the time limit." << endl;
      }
      test_score += score;
      ++test;
    }
    total_score += coeff[i] * test_score;
  }
  cout << "TOTAL SCORE: " << total_score << " (ref: 40)" << endl;
}
