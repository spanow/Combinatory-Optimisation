#include "sac2.h"
#include "main_utils.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Simple examples." << endl;

    CHECK_NEAR(Sac2(12, {4, 7, 2, 5, 5, 1, 10},
                        {3.1, 99, 0.1, 0.2, 6.6, 0.4, 111}),
               154.5, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_NEAR(Sac2(22, {4, 7, 2, 5, 5, 1, 10},
                        {3.1, 99, 0.1, 0.2, 6.6, 0.4, 111}),
               216.6, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_NEAR(Sac2(20, {4, 7, 2, 5, 5, 1, 10},
                        {3.1, 99, 0.1, 0.2, 6.6, 0.4, 111}),
               213.96, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;
  }
  {
    cout << "Corner cases." << endl;

    // Tout rentre dans le sac et il reste de la place
    CHECK_NEAR(Sac2(99, {1, 2, 3, 3, 2, 1, 1},
                      {0.4, 0.6, 0.3, 0.7, 0.2, 0.8, 0.5}),
               0.4 + 0.6 + 0.3 + 0.7 + 0.2 + 0.8 + 0.5, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;

    // Capacité zero
    CHECK_EQ(Sac2(0, {1, 2, 1}, {99, 9, 0.9}), 0);
    cout << "PASSED at line " << __LINE__ << endl;

    // Aucun objet.
    CHECK_EQ(Sac2(32, {}, {}), 0);
    cout << "PASSED at line " << __LINE__ << endl;

    // Objets de valeur nulle.
    CHECK_NEAR(Sac2(32, {1, 3, 2, 1}, {7, 0, 3, 0}), 10, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;

    // Objets de poids nul
    CHECK_NEAR(Sac2(4, {1, 2, 0, 0, 2, 0, 1},
                       {4, 7, 1, 2, 5, 0, 2}),
               3 + 7 + 4 + 2.5, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;

    // Objets de poids zero et de valeur nulle.
    CHECK_NEAR(Sac2(4, {1, 0, 2, 0, 2, 0, 1},
                       {4, 0, 3, 5, 2, 0, 1}),
               5 + 4 + 3 + 1, 1e-9);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  for (bool allow_zero_zero : {false, true}) {
    cout << "Randomized stress test " << (allow_zero_zero ? "with" : "without")
         << " (poids=0, value=0) entries" << endl;
    Random random(123);
    const int trials = 10000;
    clock_t c = 0;
    for (int trial = 0; trial < trials; ++trial) {
      const int N = 1 + int(100 * random.RandExp());
      struct PV {
        int p;
        double v;
      };
      vector<PV> pv;
      pv.reserve(N);
      int num_zeros = std::min(N, int(3 * random.RandExp()));
      int poids_total = 0;
      double value_total = 0;
      while (pv.size() < num_zeros) {
        pv.push_back({0, (allow_zero_zero && random.Uniform(2)) ? 0 : random.RandExp()});
        value_total += pv.back().v;
      }
      double solution = -1;
      int capacity = -1;
      double last_vp = random.RandExp();
      while (pv.size() < N) {
        const double div = random.Uniform(2) ? 1.0 : pow(1.0 + random.RandExp(), 1.0/N);
        CHECK_GE(div, 1);
        last_vp /= div;
        const int p = 1 + int(10 * random.RandExp());
        pv.push_back({p, p * last_vp});
        if (capacity < 0 && random.Uniform(N - num_zeros) == 0) {
          capacity = poids_total;
          solution = value_total;
          if (random.Uniform(2)) {
            capacity += random.Uniform(p);
            solution += (capacity - poids_total) * pv.back().v / p;
          }
        }
        poids_total += p;
        value_total += pv.back().v;
        CHECK_GE(poids_total, 0);  // should catch overflows unless we're unlucky.
      }
      if (capacity < 0) {
        capacity = poids_total;
        solution = value_total;
        if (random.Uniform(2)) capacity += int(1000 * random.RandExp());
      }
      shuffle(pv.begin(), pv.end(), random);
      vector<int> poids;
      vector<double> val;
      poids.reserve(N);
      val.reserve(N);
      for (const PV& x : pv) {
        poids.push_back(x.p);
        val.push_back(x.v);
      }
      c -= clock();
      const double ret = Sac2(capacity, poids, val);
      c += clock();
      const double rel_error = ret == solution ? 0 : 2 * std::abs(ret - solution) / (std::abs(ret) + std::abs(solution));
      if (rel_error > 1e-9) {
        cerr << "FAILED on trial #" << trial << ": Expected " << solution
             << ", got Sac2()=" << ret << ". Capacity=" << capacity
             << ", poids=" << poids << ", values=" << val << endl;
        return 1;
      }
    }
    double t = double(c) / CLOCKS_PER_SEC;
    cout << "Time: " << t << "s" << endl;
    cout << "PASSED at line " << __LINE__ << endl;
  }
}
