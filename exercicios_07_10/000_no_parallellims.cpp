/*---------------------------------
Exercícios
---------------------------------*/

/*
 Utilize o código base abaixo para a resolução dos exercícios.
 Ao final de cada código, escreva um parágrafo com suas palavras

 a) Qual é o objetivo do código em detalhes.
 b) O que o seu código faz, etapa por etapa em detalhes.
 c) Ao final de cada exercício, ouvirei uma explicação sua sobre o que está sendo feito.
 
 */

 /*
  Explicações, datas:

  exercício 1 e 2 hoje: 07/10/2025.
  exercício 3 e 4 14/10/2025.
  exercício 5     21/10/2025.
 */


#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>

using namespace std;

// Cria-se struct Equacao para guardar a lista de equações de forma descritiva
// Inicialmente utilizei tuple<double, double, double>, mas isso não descrevia os coeficientes a,b,c
// e causava cópias desnecessárias, já que precisava "desempacotar" a tupla em variáveis
struct Equacao
{
    double a, b, c;
};

// Função para calcular as raízes de uma lista de equações de segundo grau
// Retorna um vector pair contendo as raízes x1 e x2 de cada equação.
// Se não houver raízes reais, ambas serão 0.
vector<pair<double, double>> resolver_bhaskaras(const vector<Equacao>& eqs) {
    const int N = eqs.size();
    vector<double> deltas(N);
    vector<pair<double, double>> resultados(N);

    for (int i = 0; i < N; i++) {
        deltas[i] = (eqs[i].b * eqs[i].b) - (4 * eqs[i].a * eqs[i].c);
    }

    for (int i = 0; i < N; i++) {
        if (deltas[i] < 0) {
            resultados[i] = {0.0, 0.0};
            continue;
        }

        double x1 = (-eqs[i].b + sqrt(deltas[i])) / (2 * eqs[i].a);
        double x2 = (-eqs[i].b - sqrt(deltas[i])) / (2 * eqs[i].a);

        resultados[i] = {x1, x2};
    }

    return resultados;
}

int main() {
    // ===============================================================
    //               ÁREA PARA RESOLVER OS EXERCÍCIOS
    // ===============================================================

    // Adicione aqui o seu código OpenMP para cada exercício.
    cout << "Digite a quantidade de equações: ";

    int qtd_raizes = 0;
    cin >> qtd_raizes;

    // Preenche os vetores com valores de exemplo.
    // Metade terá raízes reais, a outra metade não.
    vector<Equacao> equacoes(qtd_raizes);

    for (int i = 0; i < qtd_raizes; ++i) {
        equacoes[i].a = 1.0;
        if (i % 2 == 0) {
            // Equação com raízes reais: x^2 - 7x + 10 = 0 (raízes 5 e 2)
            equacoes[i].b = -7.0;
            equacoes[i].c = 10.0;
        } else {
            // Equação sem raízes reais: x^2 + 2x + 5 = 0 (delta < 0)
            equacoes[i].b = 2.0;
            equacoes[i].c = 5.0;
        }
    }


    double T0 = omp_get_wtime();

    int possuem_raizes = 0;
    int nao_possuem_raizes = 0;
    vector<pair<double, double>> raizes = resolver_bhaskaras(equacoes);

    #pragma omp parallel for
    for (int i = 0; i < qtd_raizes; ++i) {
        if(raizes[i].first == 0 && raizes[i].second == 0) {
            #pragma omp critical
            {
                nao_possuem_raizes++;
            }
        }
        else {
            #pragma omp critical
            {
                possuem_raizes++;
            }
        }
    }

    double T1 = omp_get_wtime();

    double tempo_total = (T1 - T0);

    cout << setprecision(6);
    cout << "Tempo TOTAL (s): " << tempo_total << "\n";
    cout << "Equações com raízes reais: " << possuem_raizes << "\n";
    cout << "Equações sem raízes: reais: " << nao_possuem_raizes << "\n";

    // ===============================================================

    // Resultado de teste
    // Digite a quantidade de equações: 50000000
    // Tempo TOTAL (s): 12.1754
    // Equações com raízes reais: 25000000
    // Equações sem raízes: reais: 25000000

    return 0;
}

/*------------------------------------------------------
 Instruções dos exercícios:
------------------------------------------------------*/
/* 1. Faça um programa que utilize OpenMP para paralelizar a resolução das equações do segundo
grau com a formula de Bhaskara.

2. crie uma entrada de dados onde o usuário decicirá a quantidade de equações a serem resolvidas.
Ao final, mostre o tempo gasto para resolver as equações, e o número de equações que possuem raízes reais e quantas não possuem.

3.Utilizam todos os conceitos do exercios das aulas 006, na mesma ordem em qual foram apresentados.
*/