#include "algoritmo_genetico.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

AlgoritmoGenetico::AlgoritmoGenetico(int pop_tam, int gen) 
    : popular_size(pop_tam), generations(gen) {}

void AlgoritmoGenetico::inicialize_popular() {
    std::uniform_int_distribution<int> dist(0, 1);
    populacao.resize(popular_size, std::vector<int>(10));
    for (auto& individual : populacao) {
        for (auto& gene : individual) {
            gene = dist(rng);
        }
    }
}

void AlgoritmoGenetico::evaluate_popular() {
    for (auto& individual : populacao) {
        int fitness = std::count(individual.begin(), individual.end(), 1);
    }
}

void AlgoritmoGenetico::select_parents() {
    std::sort(populacao.begin(), populacao.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return std::count(a.begin(), a.end(), 1) > std::count(b.begin(), b.end(), 1);
    });
}

void AlgoritmoGenetico::crossover() {
    std::uniform_int_distribution<int> dist(0, 9);
    for (size_t i = 0; i < popular_size; i += 2) {
        int crossover_point = dist(rng);
        std::vector<int> parent1 = populacao[i];
        std::vector<int> parent2 = populacao[i + 1];
        for (int j = crossover_point; j < 10; ++j) {
            std::swap(parent1[j], parent2[j]);
        }
        populacao[i] = parent1;
        populacao[i+1] = parent2;
    }
}

void AlgoritmoGenetico::mutate() {
    std::uniform_int_distribution<int> dist(0, 9);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
    for (auto& individual : populacao) {
        for (auto& gene : individual) {
            if (prob_dist(rng) < 0.01) {
                gene = !gene;
            }
        }
    }
}

void AlgoritmoGenetico::run() {
    inicialize_popular();
    for (int i = 0; i < generations; ++i)
    {
        evaluate_popular();
        select_parents();
        crossover();
        mutate();
    }
    std::cout << "Algoritmo Genético concluído." << std::endl;
}