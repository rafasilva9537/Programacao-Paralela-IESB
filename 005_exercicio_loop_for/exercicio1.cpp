// Exercício 1 — “Hello World” Paralelo
// a) Crie uma região paralela com #pragma omp parallel.
// b) Cada thread deve imprimir uma mensagem indicando seu número (omp_get_thread_num()) e o total de threads (omp_get_num_threads()).
// c) Configure o programa para rodar com 4 threads.

#include <iostream>
#include <omp.h>

int main() {
    // Cria região com 4 threads
    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();

        // Executa uma thread por vez, evitando mistura outputs no console
        #pragma omp critical
        std::cout << "Thread " << thread_id << " de " << total_threads << " threads\n";
    }

    return 0;
}