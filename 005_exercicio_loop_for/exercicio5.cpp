// Exercício 5 — Escalonamento
// Use novamente o cálculo de a[i] = x[i]^2 + y[i]^2 + z[i]^2, mas:
// a) Execute com schedule(static) e schedule(dynamic, 1000).
// b) Compare os tempos em diferentes quantidades de threads (2, 4, 8).
// c) Explique em quais situações static e dynamic são mais adequados.

#include <iostream>
#include <vector>
#include <iomanip>
#include <omp.h>

void parallel_static(const int THREADS_NUM) {
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

    #pragma omp parallel num_threads(THREADS_NUM)
    {
        #pragma omp single // executa apenas uma vez, na primera thread utilizada
        {
            used_threads = omp_get_num_threads();
        }

        // Usado para calcular tempo da thread atual
        double local_t0 = omp_get_wtime();

        // Distribui igualmente um subconjunto de índices para cada thread
        // Determinístico, cada thread pega um bloco de quantidade igual
        #pragma omp for schedule(static)
        for (int i = 0; i < N; i++) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }

        int thread_id = omp_get_thread_num(); // pega o id atual da thread
        double local_t1 = omp_get_wtime();
        thread_times[thread_id] = local_t1 - local_t0;
    }

    double T1 = omp_get_wtime();
    double total_time = T1 - T0;

    std::cout << "Versão schedule(static)" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Threads utilizadas: " << used_threads << "\n";
    std::cout << "Tempo total: " << total_time << "s\n";

    std::cout << "Tempo por thread:\n";
    for (int i = 0; i < used_threads; i++) {
        std::cout << "  Thread " << i << ": " << thread_times[i] << "s\n";
    }
}

void parallel_dynamic(const int THREADS_NUM) {
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

    #pragma omp parallel num_threads(THREADS_NUM)
    {
        #pragma omp single // executa apenas uma vez, na primera thread utilizada
        {
            used_threads = omp_get_num_threads();
        }

        // Usado para calcular tempo da thread atual
        double local_t0 = omp_get_wtime();

        // Distribui de forma dinâmica, por demanda, um subconjunto de índices para cada thread
        // Cada thread solicita um pedaço de 1000 indíces, completa a execução, e solicita novamente outro pedaço não utilizado
        #pragma omp for schedule(dynamic, 1000)
        for (int i = 0; i < N; i++) {
            a[i] = x[i]*x[i] + y[i]*y[i] + z[i]*z[i];
        }

        int thread_id = omp_get_thread_num(); // pega o id atual da thread
        double local_t1 = omp_get_wtime();
        thread_times[thread_id] = local_t1 - local_t0;
    }

    double T1 = omp_get_wtime();
    double total_time = T1 - T0;

    std::cout << "Versão schedule(dynamic, 1000)" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Threads utilizadas: " << used_threads << "\n";
    std::cout << "Tempo total: " << total_time << "s\n";

    std::cout << "Tempo por thread:\n";
    for (int i = 0; i < used_threads; ++i) {
        std::cout << "  Thread " << i << ": " << thread_times[i] << "s\n";
    }
}

int main() {
    // c) Explique em quais situações static e dynamic são mais adequados.
        // Para operações onde o tempo de execução de cada thread é similar, 
        // como em soma de valores de vetores, usa-se o static.
        // Como cada thread já está distribuíndo igualmente os índicies 
        // e utilizar o dynamic causa um overhead a mais por conta do gerenciamento em tempo de execução
        // compensa mais usar o static.

        // Usa-se o dynamic quando se trata de operações onde há diferença notável em tempo de execução para cada thread.
        // Por exemplo, algorítmos com condicionais, onde uma thread pode ser várias vezes mais demorada, pois entrou em uma
        // condicional lenta, enquanto outra não. Nesse caso, em vez de deixar as threads que já finalizaram em espera,
        // elas pegam novos blocos de índice por demanda. 
        // Um caso seria navegar em nós de árvores, onde alguns nós são maiores que outros

    for (int thread_num = 2; thread_num <= 12; thread_num+=2) {
        std::cout << "==============================\n";
        std::cout << "Execução com " << thread_num << " threads\n";
        std::cout << "==============================\n";

        parallel_static(thread_num);
        std::cout << "\n";
        parallel_dynamic(thread_num);
        std::cout << "==============================\n";
        std::cout << "\n\n";
    }
}