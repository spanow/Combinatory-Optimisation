#include "students.h"

#include <algorithm>
#include <set>
#include "main_utils.h"
using namespace std;

struct TestCase {
  string name;
  vector<int> cours;
  vector<vector<double>> valeurs;
  double resultat;
};


int main() {

  cout.precision(12);
  vector<TestCase> test_cases = {
    {"Trivial",
      {1,1,1},// chaque cours accepte un seul étudiant
      {{0,0,10},
      {0,10,0},
      {10,0,0}},// chaque étudiant veut aller dans exactement un cours
      10},
    {"One lecture",
     {2},
     {{10},
      {10}
     }, 10},// les deux étudiants peuvent être assignés au cours
    {"Choose",
     {1, 1},
     {{4, 6},{7, 3}},6} // l'étudiant 1 aurait préféré le cours 1, mais il va avoir le 0 quand même
  };
  
  // Test randomisé géant
  Random random(999);
  const int n_etu = 500;// nombre d'étudiants
  const int n_cours = 100;// nombre de cours
  const int avg_cours_by_student = 10;
  // chaque étudiant veut aller à 10 cours en moyenne.
  // ils assignent des notes random entre 0 et 10
  
  TestCase big;
  big.name = "BigRandomized";
  for (int e = 0; e < n_cours; ++e) {
    // max 50 étudiants par cours (il faut qu'on pense au chargé de TD)
    big.cours.push_back(random.Uniform(50));
  }
  
  for (int i = 0; i < n_etu; i++) {
    vector<double> a(n_cours,0);
    double tmp = 0;
    for (int e = 0; e < n_cours; ++e) {
      double newval = random.RandDouble()*10.0/avg_cours_by_student;
      if (random.Uniform(n_cours) < avg_cours_by_student && tmp + newval < 10) {
        a[e] = newval;
        tmp += newval;
      }
      if (tmp + newval > 10) {
        break;
      }
    }
    // compléter avec le reste
    double finalval = 10 - tmp;
    a[random.Uniform(n_cours)] += finalval;
    
    big.valeurs.push_back(a);
  }
  

  big.resultat = 3.69722874998;
  test_cases.push_back(big);

  for (const TestCase& t : test_cases) {
    cout << endl << "=========================================================="
         << endl << "Testing '" << t.name << "'..." << endl;
    
    double sol = StudentsAssignment(t.cours, t.valeurs);
    cout << sol << endl;
    if (sol < t.resultat - 1e-3) {
      cout << "FAILED: Your solution is SUBOPTIMAL! (Result: " << sol
           << ", Optimal: " << t.resultat << ")" << endl;
    } else {
      if (sol > t.resultat + 1e-3) {
        cout << "ANORMAL: Your solution is better than expected! (Result: " << sol
           << ", Optimal: " << t.resultat << ")" << endl;
    }
     else {
      cout << "PASSED: You found an optimal solution!" << endl;
    }
    }
  }
}
