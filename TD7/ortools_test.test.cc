#include "ortools_test.h"
#include "main_utils.h"

using namespace std;

int main() {
    cout << "Test de or-tools: on va faire quelques calculs simples" << endl;
    cout << "Vous devriez voir des tableaux avec une moitié de '1' " << endl;
    // sur cet exemple: on minimise x1 + x2 + x3 + x4 sachant que: x1 + x4 >= 1 et x2 + x3 >= 1
    // (une) solution est donc de prendre une moitié à 1 et l'autre à 0
    cout << "Solution pour 4: " << endl;
    cout << MonPremierSolveur(4) << endl;
    cout << "Solution pour 5: " << endl;
    cout << MonPremierSolveur(5) << endl;
    
    cout << "Solution pour 8: " << endl;
    cout << MonPremierSolveur(8) << endl;
    
    cout << "Ça marche !" << endl;
}


