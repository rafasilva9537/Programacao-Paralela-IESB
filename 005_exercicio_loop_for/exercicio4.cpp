// Exercício 4 — Medindo tempo por thread
// Adapte o código do exercício anterior para:
// a) Medir e exibir o tempo total de execução.
// b) Medir e exibir o tempo gasto por cada thread.
// c) Mostrar quantas threads foram utilizadas no cálculo.

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
        double local_t0 = omp_get_wtime();
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

    // Cria lista para guardar tempos de cada thread
    // omp_get_max_threads() aloca o "número máximo de threads" elementos
    std::vector<double> thread_times(omp_get_max_threads(), 0.0);
    int used_threads = 0;

    double T0 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single // executa apenas uma vez, na primera thread utilizada
        {
            used_threads = omp_get_num_threads();
        }

        // Usado para calcular tempo da thread atual
        double local_t0 = omp_get_wtime();

        #pragma omp for schedule(static) // distribui igualmente um subconjunto de índices para cada thread
        for (int i = 0; i < N; i++) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }

        int thread_id = omp_get_thread_num(); // pega o id atual da thread
        double local_t1 = omp_get_wtime();
        thread_times[thread_id] = local_t1 - local_t0;
    }

    double T1 = omp_get_wtime();
    double total_time = T1 - T0;

    std::cout << "Versão paralela" << std::endl;
    std::cout << std::fixed << std::setprecision(16);
    std::cout << "Threads utilizadas: " << used_threads << "\n";
    std::cout << "Tempo total: " << total_time << "s\n";

    std::cout << "Tempo por thread:\n";
    for (int i = 0; i < used_threads; ++i) {
        std::cout << "  Thread " << i << ": " << thread_times[i] << "s\n";
    }
}

int main() {
    sequential_version();
    std::cout << "\n\n";
    parallel_static();

    // Resultado teste
    // Versão sequencial
    // Tempo total: 0.0366425399997752s
    // 
    // 
    // Versão paralela
    // Threads utilizadas: 12
    // Tempo total: 0.0151104799997483s
    // Tempo por thread (s):
    //   Thread 0: 0.0079898499998308
    //   Thread 1: 0.0054212400000324
    //   Thread 2: 0.0054214599995248
    //   Thread 3: 0.0054215700001805
    //   Thread 4: 0.0054129899999680
    //   Thread 5: 0.0054209099998843
    //   Thread 6: 0.0054206899994824
    //   Thread 7: 0.0054212400000324
    //   Thread 8: 0.0054209099998843
    //   Thread 9: 0.0054211300002862
    //   Thread 10: 0.0054170599996723
    //   Thread 11: 0.0014359400001922
}