#include "ortools_test.h"
#include <string>
#include "ortools/linear_solver/linear_solver.h"

// ce fichier contient un exemple simple d'utilisation d'un solveur MILP
// il vous permettra de vérifier que or_tools est bien installé, et par ailleurs,
// ça vous montre aussi comment l'utiliser.

using namespace std;
using namespace operations_research;

vector<double> MonPremierSolveur(int n) {
    // voici le problème qu'on va résoudre: on prend n variables
    // allant de 0 à l'infini: x1 ... xn
    // on veut minimiser la somme de x1 ... xn sachant que, pour tout (xi, xj)
    // avec i + j == n+1, xi + xj doit être supérieur ou égal à 1.

    // Commençons par créer un solveur
    MPSolver solver("Mon premier solveur", MPSolver::GLOP_LINEAR_PROGRAMMING);

    // on donne une valeur à l'infini
    const double infinity = solver.infinity();

    // l'objectif sera de minimiser une fonction linéaire des variables
    // (en l'occurrence, la somme)
    MPObjective* objective = solver.MutableObjective();
    objective->SetMinimization();

    // on définit maintenant nos n variables x1 ... xn
    // ATTENTION: les variables doivent avoir des noms distincts (d'où l'usage
    // de std::to_string)
    vector<MPVariable*> x;
    for (int i = 0; i < n; i++) {
        MPVariable* var = solver.MakeNumVar(0, infinity, "x" + std::to_string(i));
        x.push_back(var);
    }

    // maintenant, on va mettre le coefficient de chaque variable dans l'objectif: 1
    for (int i = 0; i < n; i++) {
        objective->SetCoefficient(x[i], 1);
    }

    // maintenant, on va créer les contraintes: une contrainte, c'est
    // une combinaison linéaire des variables qui doit être entre deux bornes.
    for (int i = 0; i < n; i++) {
        int j = n-i-1;
        MPConstraint* newcons = solver.MakeRowConstraint(1, infinity);
        newcons->SetCoefficient(x[i],1);
        newcons->SetCoefficient(x[j],1);
    }

    // on résout
    solver.Solve();

    // une fois le problème résolu, chaque variable va admettre une solution_value()
    // on renvoie le résultat final
    vector<double> solution;
    for (MPVariable* v : x) {
        solution.push_back(v->solution_value());
    }
    return solution;
}
