#include "bfs.1.h"
#include "bfs.2.h"
#include "ugraph.h"
#include "diam1.h"
#include "main_utils.h"
#include "graph_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
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
  srandom(0);
  Random random(123);
  cout << "Performance tests on a few graphs\n";
  cout << "NOTE: you'll get 0 overall if there is a bug (segfault, out of bounds answer), even if it only happens in 1 test" << endl;
  cout << "NOTE: trespassing the time limit means 0 score for that test." << endl;

  vector<UndirectedGraph> graphs;
  graphs.push_back(LargestComponent(ErdosRenyi(1000, 1000, /*force_simple=*/true, &random)));
  graphs.push_back(LargestComponent(ErdosRenyi(10000, 13000, /*force_simple=*/true, &random)));
  graphs.push_back(LargestComponent(ErdosRenyi(100000, 110000, /*force_simple=*/true, &random)));
  graphs.push_back(LargestComponent(GcdGraph(1000, 3)));
  graphs.push_back(LargestComponent(GcdGraph(1000, 20)));
  graphs.push_back(LargestComponent(GcdGraph(1000, 100)));
  graphs.push_back(LargestComponent(GcdGraph(3000, 2)));
  graphs.push_back(LargestComponent(GcdGraph(10000, 1000)));
  graphs.push_back(BarabasiAlbert(1000, 1000, &random));
  graphs.push_back(BarabasiAlbert(1000, 1100, &random));
  graphs.push_back(BarabasiAlbert(10000, 13000, &random));
  graphs.push_back(BarabasiAlbert(100000, 120000, &random));
  graphs.push_back(BarabasiAlbert(100000, 200000, &random));

  const int repeat = 1;
  double total_score = 0;
  const int ref[] = {21,25,32,5,25,106,4,1011,15,15,16,20,14};
  const double coeff[] = {1.5,1.5,1.0,0.5,0.5,0.5,0.5,0.5,1.0,1.0,1.0,1.5,1.0};
  for (int i = 0; i < graphs.size(); ++i) {
    const UndirectedGraph& g = graphs[i];
    const double max_time = 1e-6 * (g.NumNodes() + g.NumEdges()) + 50e-3;
    clock_t c0 = clock();
    pair<int, int> diam = DiametreLB(g);
    CHECK_GE(diam.first, 0);
    CHECK_LT(diam.first, g.NumNodes());
    CHECK_GE(diam.second, 0);
    CHECK_LT(diam.second, g.NumNodes());
    for (int i = 1; i < repeat; ++i) {
      const pair<int, int> diam = DiametreLB(g);
      CHECK_GE(diam.first, 0);
      CHECK_LT(diam.first, g.NumNodes());
      CHECK_GE(diam.second, 0);
      CHECK_LT(diam.second, g.NumNodes());
    }
    const double t = double(clock() - c0) / CLOCKS_PER_SEC / repeat;

    const int d = GetBfsDistances(BFS(g, diam.first))[diam.second];
    printf("TEST #%2d: Diametre >= %4d (ref: %4d), time = %.1lf%% of limit.\n",
           i, d, ref[i], 0.1 * int(1000 * t / max_time));
    double score = d > ref[i] ? 3.0 : d == ref[i] ? 1.0 : pow(0.5, ref[i] - d);
    if (t > max_time) {
      score = 0;
      cout << "ERROR: Time limit trespassed! Scored 0" << endl;
    } else if (t >= 0.5 * max_time) {
      cout << "WARNING: the time is more than 50% the time limit. This may be scored 0" << endl;
    }
    total_score += coeff[i] * score;
  }
  cout << "TOTAL SCORE: " << total_score * 3.0 << " (ref: 36)" << endl;
}
