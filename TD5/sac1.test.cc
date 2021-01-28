#include "sac1.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Simple examples." << endl;

    CHECK_EQ(Sac1(12, {3, 7, 2, 1, 8, 4, 3}), 4);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(Sac1(13, {3, 7, 2, 1, 8, 4, 3}), 5);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(Sac1(32, {3, 5, 7, 2, 1, 8, 4, 3, 5, 4, 6, 2, 8}), 9);
    cout << "PASSED at line " << __LINE__ << endl;
  }
  {
    cout << "Corner cases." << endl;

    // Objets de poids nul
    CHECK_EQ(Sac1(4, {1, 2, 0, 0, 2, 0, 1}), 6);
    cout << "PASSED at line " << __LINE__ << endl;

    // Tout rentre dans le sac et il reste de la place
    CHECK_EQ(Sac1(99, {1, 2, 0, 0, 2, 0, 1}), 7);
    cout << "PASSED at line " << __LINE__ << endl;

    // Capacité zero
    CHECK_EQ(Sac1(0, {1, 2, 2, 1}), 0);
    cout << "PASSED at line " << __LINE__ << endl;

    // Capacité zero avec objets de poids zero.
    CHECK_EQ(Sac1(0, {1, 0, 2, 0, 2, 0, 1}), 3);
    cout << "PASSED at line " << __LINE__ << endl;

    // Aucun objet.
    CHECK_EQ(Sac1(32, {}), 0);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Randomized stress test" << endl;
    Random random(123);
    const int trials = 10000;
    clock_t c = 0;
    for (int trial = 0; trial < trials; ++trial) {
      const int N = 1 + int(100 * random.RandExp());
      const int K = random.Uniform(N + 1);
      vector<int> objets;
      objets.reserve(N);
      int capa = 0;
      while (objets.size() < N) {
        objets.push_back((objets.empty() ? 0 : objets.back()) + int(2 * random.RandExp()));
        if (objets.size() <= K) capa += objets.back();
        if (objets.size() == K+1) {
          if (objets.back() == 0) objets.back() = 1;
          capa += random.Uniform(objets.back());
        }
        CHECK_GE(capa, 0);  // should catch overflows unless we're unlucky.
      }
      if (K == N) capa += int(1000 * random.RandExp());
      shuffle(objets.begin(), objets.end(), random);
      c -= clock();
      const int k = Sac1(capa, objets);
      c += clock();
      if (K != k) {
        cerr << "FAILED on trial #" << trial << ": Expected " << K
             << ", got Sac1()=" << k << ". Capacity=" << capa
             << ", obj=" << objets << endl;
        return 1;
      }
    }
    double t = double(c) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s" << endl;
    cout << "PASSED at line " << __LINE__ << endl;
  }
}
