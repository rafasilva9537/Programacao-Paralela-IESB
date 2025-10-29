#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>

struct Pessoa {
    double estatura;
    double peso;
};

using namespace std;

int main() {
    double maiorEstatura = 190;
    double menorEstatura = 150;
    double amplitudeEstatura = maiorEstatura - menorEstatura;

    double maiorPeso = 120;
    double menorPeso = 30;
    double amplitudePeso = maiorPeso - menorPeso;

    // qtd população
    int N = 1000;

    int k = 1 + 3.3 * log10(N); // 

    double intervaloEstatura = amplitudeEstatura / k;
    int intervaloPeso = amplitudePeso / k;

    cout << "Intervalo Estatura: " << intervaloEstatura << "\n";

    return 0;
}