#include "menu.h"
#include "ortools/linear_solver/linear_solver.h"

using namespace operations_research;

vector<double> MenuMoinsCher(const vector<double>& ajr,
                             const vector<vector<double>>& apports_par_ingredient,
                             const vector<double>& prix_par_ingredient) {
  MPSolver solver("Mon Menu Bof", MPSolver::GLOP_LINEAR_PROGRAMMING);
  const int num_ingredients = apports_par_ingredient.size();
  const int num_elements = ajr.size();

  // CONSEILS -- BIEN LES LIRE EN ENTIER!
  // - Regarder le fichier "../examples/cpp/linear_programming.cc" pour voir
  //   comment définir des variables, des contraintes, et l'objectif.
  // - Contrairement a l'exemple, on a pas un nombre constant, prédéfini, de
  //   variables et de contraintes: elles dépendent de l'entrée. Il peut
  //   être utile de créér un tableau de MPVariable* stockant les variables
  //   créées pour modéliser le problème. Idem pour les contraintes.
  //   Et on les crééra dans des boucles "for" a priori.
  // - N'oubliez pas qu'après Solve() il faut encore renvoyer la solution
  //   dans le format demandé!

  // Pour dire qu'il va a l'infinie.
  const double infinity = solver.infinity();

  // L'objectif est de minimiser le prix du menu, on mettera les coeff plus tard.
  MPObjective*  const objectif = solver.MutableObjective();
  objectif->SetMinimization();

  // Creation des contraintes et variables dans un tableau à l'aide boucle.
  vector<MPVariable*> all_ingredients;
  for (int i = 0; i < num_ingredients; i++){
    MPVariable* const variable = solver.MakeNumVar(0.0, infinity, "#"+ std::to_string(i));//On donne le nom de ième ingredient.
    all_ingredients.push_back(variable);
    objectif->SetCoefficient(variable, prix_par_ingredient[i]);
  }

  // Definition des contraintes en fonction des all_ingredient et AJR.
  //contrainte: la somme des ingrédients * son apports_par_ingredient
  //soit >= AJR
  //(de l'ingrédient en question).

  for(int e = 0; e < num_elements; e++){
    // Pris de l'example de solver Google
    MPConstraint* const c = solver.MakeRowConstraint(ajr[e], infinity);
    for (int i = 0; i < num_ingredients; i++){
      c->SetCoefficient(all_ingredients[i], apports_par_ingredient[i][e]);
    }
  }
  solver.Solve();
  vector<double> menu_bof;
  for (MPVariable* m : all_ingredients){
    // Comme dans l'example.
    menu_bof.push_back(m->solution_value());
  }
  return menu_bof;
}
