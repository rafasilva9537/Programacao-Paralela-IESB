// Exercício 3 — Expressão Vetorial
// Implemente a expressão: a[i] = x[i]^2 + y[i]^2 + z[i]^2 para vetores de tamanho 1.000.000.
// a) Faça a versão sequencial.
// b) Paralelize com #pragma omp parallel for schedule(static).
// c) Mostre o tempo total de execução em cada versão.

#include <iostream>
#include <vector>
#include <iomanip>
#include <omp.h>

void sequential_version() {
    const int N = 1'000'000;
    std::vector<int> x(N), y(N), z(N), a(N);

    // Inicializa valores
    for (int i = 0; i < N; i++) {
        x[i] = i;
        y[i] = i + 1;
        z[i] = i + 2;
    }

    double T0 = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    }
    double T1 = omp_get_wtime();
    double total_time = T1 - T0;

    std::cout << "Versão sequencial" << std::endl;
    std::cout << std::fixed << std::setprecision(16);
    std::cout << "Tempo total: " << total_time << "s" << std::endl;
}

void parallel_static() {
    const int N = 1'000'000;
    std::vector<int> x(N), y(N), z(N), a(N);

    // Inicializa valores
    for (int i = 0; i < N; i++) {
        x[i] = i;
        y[i] = i + 1;
        z[i] = i + 2;
    }

    double T0 = omp_get_wtime();
    #pragma omp parallel for schedule(static) // distribui igualmente um subconjunto de índices para cada thread
    for (int i = 0; i < N; i++) {
        a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
    }
    double T1 = omp_get_wtime();
    double total_time = T1 - T0;

    std::cout << "Versão paralela" << std::endl;
    std::cout << std::fixed << std::setprecision(16);
    std::cout << "Tempo total: " << total_time << "s" << std::endl;
}

int main() {
    sequential_version();
    std::cout << "\n\n";
    parallel_static();

    // Resultado teste
    // Versão sequencial
    // Tempo total: 0.0111204340000768s
    // 
    // 
    // Versão paralela
    // Tempo total: 0.0098639779998848s    
}