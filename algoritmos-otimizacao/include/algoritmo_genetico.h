#ifndef ALGORITMO_GENETICO_H
#define ALGORITMO_GENETICO_H

#include <vector>

class AlgoritmoGenetico {
public:
    AlgoritmoGenetico(int popular_size, int generations);
    void run();

private:
    int popular_size;
    int generations;
    void inicialize_popular();
    void evaluate_popular();
    void select_parents();
    void crossover();
    void mutate();

    std::vector<std::vector<int>> populacao;
};

#endif