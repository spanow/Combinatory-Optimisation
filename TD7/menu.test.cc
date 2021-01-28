#include "menu.h"

#include "main_utils.h"
using namespace std;

struct TestCase {
  string name;
  vector<double> ajr;
  vector<vector<double>> apports_par_ingredient;
  vector<double> prix_par_ingredient;
  double prix_optimal;
};

int main() {
  cout.precision(12);
  vector<TestCase> test_cases = {
    {"Trivial",
     {2000.0},
     {{500.}},
     {10.},
     40.},
    {"TrivialZero",
     {0.0},
     {{500.}},
     {10.},
     0.},
    {"TrivialFloat",
     {12.34},
     {{24.68}},
     {3.2},
     1.6},
    {"Small",
     {2000.0, 43.2, 0.1},
     {{240., 12.1, 0.001},
      {70.0, 0.1, 1e-5},
      {5.5, 14.3, 0.3},
      {1.0, 0.01, 0.0001}
     },
     {30, 0.7, 50.0, 1.0},
     119.99392518},
  };
  // Add a big, randomized test case.
  Random random(123);
  const int n_el = 1000;
  const int n_ing = 20000;
  const int avg_num_el_by_ing = 10;
  TestCase big;
  big.name = "BigRandomized";
  for (int e = 0; e < n_el; ++e) {
    big.ajr.push_back(random.RandDouble() * 100.0);
  }
  for (int i = 0; i < n_ing; ++i) {
    vector<double> a(n_el, 0);
    for (int e = 0; e < n_el; ++e) {
      if (random.Uniform(n_el) < avg_num_el_by_ing) {
        a[e] = random.RandDouble();
      }
    }
    big.apports_par_ingredient.push_back(a);
    big.prix_par_ingredient.push_back(random.RandDouble() * 10.0);
  }
  big.prix_optimal = 19871.5153492;
  test_cases.push_back(big);

  for (const TestCase& t : test_cases) {
    cout << endl << "=========================================================="
         << endl << "Testing '" << t.name << "'..." << endl;
    const vector<double> sol = MenuMoinsCher(t.ajr, t.apports_par_ingredient, t.prix_par_ingredient);
    const int n_ing = t.prix_par_ingredient.size();
    const int n_el = t.ajr.size();
    // Verify that it's a solution!
    CHECK_EQ(sol.size(), n_ing);
    double price = 0;
    for (int i = 0; i < n_ing; ++i) {
      if (sol[i] < -1e-5) {
        cerr << "ERROR: In your solution, ingredient #" << i
             << " has a negative quantity!" << endl;
        return 1;
      }
      price += sol[i] * t.prix_par_ingredient[i];
    }
    for (int e = 0; e < n_el; ++e) {
      double apport = 0;
      for (int i = 0; i < n_ing; ++i) {
        apport += sol[i] * t.apports_par_ingredient[i][e];
      }
      if (apport < t.ajr[e]-1e-5) {
        cerr << "ERROR: In your solution, the total contribution to element #" << e
             << " (" << apport << ") is lower than AJR[" << e << "] = " << t.ajr[e] << endl;
        return 1;
      }
    }
    cout << "Your solution is feasible. That's good." << endl;
    if (price > t.prix_optimal + 1e-3) {
      cout << "FAILED: Your solution is SUBOPTIMAL! (Total price: " << price
           << ", Optimal: " << t.prix_optimal << ")" << endl;
    } else {
      cout << "PASSED: You found an optimal solution!" << endl;
    }
  }
}
