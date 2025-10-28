#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>
#include <string>

using namespace std;

int main() {
    // Nome da BigTech
    string bigtech = "NeoTech";

    // Departamentos e cargos
    vector<pair<string, vector<string>>> deptos_cargos = {
        {"RH", {"Assistente", "Analista", "Gerente"}},
        {"Marketing", {"Analista", "Coordenador", "Gerente"}},
        {"Diretoria", {"Diretor Executivo", "Diretor Financeiro"}},
        {"Suporte", {"Júnior", "Pleno", "Sênior"}}
    };

    const int N = 2'000'000; // total de funcionários
    vector<double> salarios(N);
    vector<string> departamentos(N);
    vector<string> cargos(N);

    // Gerar salários determinísticos
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        int dep_idx = i % deptos_cargos.size();
        int cargo_idx = i % deptos_cargos[dep_idx].second.size();

        departamentos[i] = deptos_cargos[dep_idx].first;
        cargos[i] = deptos_cargos[dep_idx].second[cargo_idx];

        // Salário determinístico: 1.5k a 20k
        double fator = i % 18501;        // varia de 0 a 18.500
        salarios[i] = 1500 + fator;      // 1.500 a 20.000
    }

    // 1. Média salarial
    double T0 = omp_get_wtime();

    double soma = 0.0;
    #pragma omp parallel for reduction(+:soma)
    for (int i = 0; i < N; i++) {
        soma += salarios[i];
    }
    double media = soma / N;


    // 2. Variância amostral
    double soma_quadrados = 0.0;
    #pragma omp parallel for reduction(+:soma_quadrados)
    for (int i = 0; i < N; i++) {
        double diff = salarios[i] - media;
        soma_quadrados += diff * diff;
    }
    double variancia = soma_quadrados / (N - 1);


    // 3. Desvio padrão
    double desvio_padrao = sqrt(variancia);

    double T1 = omp_get_wtime();
    double tempo_total = (T1 - T0);

    // Resultado
    cout << setprecision(6);
    cout << "Tempo TOTAL (s): " << tempo_total << "\n";
    cout << "BigTech: " << bigtech << "\n";
    cout << "Número de funcionários: " << N << "\n";
    cout << "Média salarial: R$ " << media << "\n";
    cout << "Desvio padrão amostral: R$ " << desvio_padrao << "\n";

    // Print testado
    // Tempo TOTAL (s): 0.00208139
    // BigTech: NeoTech
    // Número de funcionários: 2000000
    // Média salarial: R$ 10742.1
    // Desvio padrão amostral: R$ 5344.38

    // BEM MAIS RÁPIDO QUE USANDO locks

    return 0;
}