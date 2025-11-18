#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>
#include <random>

using namespace std;

int main() {
    // início da medição de tempo
    double t_inicio = omp_get_wtime();

    double maiorEstatura = 190;
    double menorEstatura = 150;
    double mediaEstatura = (maiorEstatura + menorEstatura) / 2;

    double maiorPeso = 120;
    double menorPeso = 30;
    double mediaPeso = (maiorPeso + menorPeso) / 2;

    // desvio padrão proporcional à amplitude
    double desvioDadosEstatura = (maiorEstatura - menorEstatura) / 6.0;
    double desvioDadosPeso = (maiorPeso - menorPeso) / 6.0;

    int N = 1000;

    // usando Sturges para definir número de classes
    int qtdClasses = ceil(1 + 3.3 * log10(N));


    // GERAR DADOS DETERMINÍSTICOS (com seed fixa)
    vector<double> estaturas(N);
    vector<double> pesos(N);

    mt19937 gen(42); // seed fixa
    normal_distribution<> distEst(mediaEstatura, desvioDadosEstatura);
    normal_distribution<> distPeso(mediaPeso, desvioDadosPeso);

    for (int i = 0; i < N; i++) {
        // passa a engine 'mt19937' de números aleatórios
        double estatura = distEst(gen);
        double peso = distPeso(gen);

        // fixo dentro dos limites definidos
        if (estatura < menorEstatura) estatura = menorEstatura;
        if (estatura > maiorEstatura) estatura = maiorEstatura;

        if (peso < menorPeso) peso = menorPeso;
        if (peso > maiorPeso) peso = maiorPeso;

        estaturas[i] = estatura;
        pesos[i] = peso;
    }


    // AGRUPAR EM CLASSES
    double intervaloEstatura = (maiorEstatura - menorEstatura) / qtdClasses;
    double intervaloPeso = (maiorPeso - menorPeso) / qtdClasses;

    vector<int> freqEst(qtdClasses, 0);
    vector<int> freqPeso(qtdClasses, 0);

    // frequências de estatura
    for (double estatura : estaturas) {
        int idx = (int)((estatura - menorEstatura) / intervaloEstatura);
        if (idx == qtdClasses) idx = qtdClasses - 1; // protege limite superior
        freqEst[idx]++;
    }

    // frequências de peso
    for (double peso : pesos) {
        int idx = (int)((peso - menorPeso) / intervaloPeso);
        if (idx == qtdClasses) idx = qtdClasses - 1;
        freqPeso[idx]++;
    }


    // IMPRIMIR FREQUÊNCIAS AGRUPADAS
    cout << fixed << setprecision(2);

    cout << "\nFrequências de Estatura\n";
    for (int i = 0; i < qtdClasses; i++) {
        double inicio = menorEstatura + i * intervaloEstatura;
        double fim = inicio + intervaloEstatura;
        cout << i << ". [" << inicio << " - " << fim << "]: " << freqEst[i] << "\n";
    }

    cout << "\nFrequências de Peso\n";
    for (int i = 0; i < qtdClasses; i++) {
        double inicio = menorPeso + i * intervaloPeso;
        double fim = inicio + intervaloPeso;
        cout << i << ". [" << inicio << " - " << fim << "]: " << freqPeso[i] << "\n";
    }


    // CALCULAR PONTOS MÉDIOS DAS CLASSES
    vector<double> pontosMediosEstatura(qtdClasses);
    vector<double> pontosMediosPeso(qtdClasses);

    #pragma omp parallel for
    for (int i = 0; i < qtdClasses; i++) {
        // Ex: [150 - 154] -> (154 - 150) / 2 = 152
        double inicioEst = menorEstatura + i * intervaloEstatura;
        double fimEst = inicioEst + intervaloEstatura;
        pontosMediosEstatura[i] = (inicioEst + fimEst) / 2.0;

        double inicioPeso = menorPeso + i * intervaloPeso;
        double fimPeso = inicioPeso + intervaloPeso;
        pontosMediosPeso[i] = (inicioPeso + fimPeso) / 2.0;
    }


    // CALCULAR MÉDIAS PONDERADAS
    double mediaPonderadaEstatura = 0.0;
    double mediaPonderadaPeso = 0.0;

    #pragma omp parallel for reduction(+:mediaPonderadaEstatura)
    for (int i = 0; i < qtdClasses; i++) {
        mediaPonderadaEstatura += pontosMediosEstatura[i] * freqEst[i];
    }
    mediaPonderadaEstatura = mediaPonderadaEstatura / N;

    #pragma omp parallel for reduction(+:mediaPonderadaPeso)
    for (int i = 0; i < qtdClasses; i++) {
        mediaPonderadaPeso += pontosMediosPeso[i] * freqPeso[i];
    }
    mediaPonderadaPeso = mediaPonderadaPeso / N;

    cout << "\nMédia Ponderada Estatura: " << mediaPonderadaEstatura << " cm\n";
    cout << "Média Ponderada Peso: " << mediaPonderadaPeso << " kg\n\n";


    // CALCULAR DESVIO PADRÃO POPULACIONAL
    double dpEstatura = 0.0;
    double dpPeso = 0.0;

    #pragma omp parallel for reduction(+:dpEstatura)
    for (int i = 0; i < qtdClasses; i++) {
        dpEstatura += freqEst[i] * pow(pontosMediosEstatura[i] - mediaPonderadaEstatura, 2);
    }
    dpEstatura = sqrt(dpEstatura / N);

    #pragma omp parallel for reduction(+:dpPeso)
    for (int i = 0; i < qtdClasses; i++) {
        dpPeso += freqPeso[i] * pow(pontosMediosPeso[i] - mediaPonderadaPeso, 2);
    }
    dpPeso = sqrt(dpPeso / N);


    // CALCULAR COEFICIENTE DE VARIAÇÃO
    double cvEstatura = (dpEstatura / mediaPonderadaEstatura) * 100.0;
    double cvPeso = (dpPeso / mediaPonderadaPeso) * 100.0;

    // fim da medição de tempo
    double t_fim = omp_get_wtime();

    cout << fixed << setprecision(6);
    cout << "Tempo de execução: " << t_fim - t_inicio << " segundos\n";
    cout << fixed << setprecision(2);
    cout << "Desvio Padrão Estatura: " << dpEstatura << " cm\n";
    cout << "Desvio Padrão Peso: " << dpPeso << " kg\n";
    cout << "Coeficiente de Variação Estatura: " << cvEstatura << " %\n";
    cout << "Coeficiente de Variação Peso: " << cvPeso << " %\n";

    return 0;
}
