#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>
#include <string>

using namespace std;

int main() {
    string bigtech = "NeoTech";

    vector<pair<string, vector<string>>> deptos_cargos = {
        {"RH", {"Assistente", "Analista", "Gerente"}},
        {"Marketing", {"Analista", "Coordenador", "Gerente"}},
        {"Diretoria", {"Diretor Executivo", "Diretor Financeiro"}},
        {"Suporte", {"Júnior", "Pleno", "Sênior"}}
    };

    const int N = 2'000'000;
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

        salarios[i] = 1500 + (i % 18501); // 1.500 a 20.000
    }

    // 1. Média salarial usando lock
    double T0 = omp_get_wtime();

    double soma = 0.0;
    omp_lock_t lock_media;
    omp_init_lock(&lock_media);

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        omp_set_lock(&lock_media);
        soma += salarios[i];
        omp_unset_lock(&lock_media);
    }
    omp_destroy_lock(&lock_media);

    double media = soma / N;


    // 2. Variância amostral usando lock

    double soma_quadrados = 0.0;
    omp_lock_t lock_variancia;
    omp_init_lock(&lock_variancia);

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        double diff = salarios[i] - media;
        omp_set_lock(&lock_variancia);
        soma_quadrados += diff * diff;
        omp_unset_lock(&lock_variancia);
    }
    omp_destroy_lock(&lock_variancia);

    double variancia = soma_quadrados / (N - 1);

    // 3. Desvio padrão
    double desvio_padrao = sqrt(variancia);

    double T1 = omp_get_wtime();
    double tempo_total = (T1 - T0);

    cout << setprecision(6);
    cout << "Tempo TOTAL (s): " << tempo_total << "\n";
    cout << "BigTech: " << bigtech << "\n";
    cout << "Número de funcionários: " << N << "\n";
    cout << "Média salarial: R$ " << media << "\n";
    cout << "Desvio padrão amostral: R$ " << desvio_padrao << "\n";

    // Print teste
    // Tempo TOTAL (s): 1.28367
    // BigTech: NeoTech
    // Número de funcionários: 2000000
    // Média salarial: R$ 10742.1
    // Desvio padrão amostral: R$ 5344.38

    return 0;
}