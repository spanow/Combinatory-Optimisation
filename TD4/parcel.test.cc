#include "parcel.h"
#include "main_utils.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

int main() {
  {
    cout << "Simple examples." << endl;

    CHECK_EQ(MaxGain({3, 7, 2, 1, 8, 4, 3}), 18);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain(vector<int>(100, 42)), 50 * 42);
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain(vector<int>(101, 42)), 51 * 42);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Corner cases." << endl;

    CHECK_EQ(MaxGain({5, 9}), 9);  // Only 2 parcels
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain({5}), 5);  // Only one parcel
    cout << "PASSED at line " << __LINE__ << endl;

    CHECK_EQ(MaxGain({}), 0);  // Empty
    cout << "PASSED at line " << __LINE__ << endl;

    // All parcels are worth nothing.
    CHECK_EQ(MaxGain({0, 0, 0, 0, 0, 0, 0, 0}), 0);
    cout << "PASSED at line " << __LINE__ << endl;
  }

  {
    cout << "Performance + stress test." << endl;
    vector<int> gains;
    const int size = 10000;
    int expected_best = 0;
    while (gains.size() < size) {
      gains.push_back(rand() % 10);
      const int big = 20 + rand() % 20;
      expected_best += big;
      gains.push_back(big);
      if (rand() % 2) gains.push_back(rand() % 10);
    }
    clock_t c0 = clock();
    const int result = MaxGain(gains);
    const double t = double(clock() - c0) / CLOCKS_PER_SEC;
    cout << "Time for N=" << size << ": " << t << " s." << endl;
    CHECK_EQ(result, expected_best);
    cout << "PASSED at line " << __LINE__ << endl;
    if (t < 0.002) {
      cout << "NICE! Complexity is probably linear!" << endl;
    } else if (t < 1) {
      cout << "NOT IDEAL: The complexity is probably quadratic. That's good, but you can do better!" << endl;
    } else {
      cout << "SLOW: The complexity seems exponential... Try again!" << endl;
    }
  }
}
