#include <iostream>
#include "algoritmo_genetico.h"

int main() {
    
    int populacao_size = 100;
    int generations = 50;

    AlgoritmoGenetico ga(populacao_size, generations);
    ga.run();

    std::cout << "Teste concluído" << std::endl;
    
    return 0;
}