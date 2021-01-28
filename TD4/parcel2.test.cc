#include "parcel2.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Simple examples." << endl;

    CHECK_EQ(ToStr(OptimalParcels({2, 5, 2})), "[1]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(OptimalParcels({2, 3, 2})), "[0 2]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(OptimalParcels({3, 7, 2, 1, 8, 4, 3})),
             "[1 4 6]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(OptimalParcels(vector<int>(19, 42))),
             "[0 2 4 6 8 10 12 14 16 18]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Corner cases." << endl;

    CHECK_EQ(ToStr(OptimalParcels({5, 9})), "[1]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(OptimalParcels({9, 5})), "[0]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(OptimalParcels({2})), "[0]");
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(ToStr(OptimalParcels({})), "[]");
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Performance + stress test." << endl;
    vector<int> gains;
    const int size = 10000;
    vector<int> expected;
    while (gains.size() < size) {
      gains.push_back(rand() % 10);
      const int big = 20 + rand() % 20;
      expected.push_back(gains.size());
      gains.push_back(big);
      if (rand() % 2) gains.push_back(rand() % 10);
    }
    clock_t c0 = clock();
    const vector<int> result = OptimalParcels(gains);
    const double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time for N=" << size << ": " << t << " s." << endl;
    CHECK_EQ(result, expected);
    cout << "PASSED at line " << __LINE__ << endl;
    if (t < 0.004) {
      cout << "NICE! Complexity is probably linear!" << endl;
    } else if (t < 1) {
      cout << "NOT IDEAL: The complexity is probably quadratic. That's good, but you can do better!" << endl;
    } else {
      cout << "SLOW: The complexity seems exponential... Try again!" << endl;
    }
  }
}
