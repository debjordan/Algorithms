#include <iostream>
#include <cmath>
#include "particle_swarm_optimization.h"

// Função de teste 1: Esfera (mínimo global em [0,0,...,0] = 0)
double sphere_function(const std::vector<double>& x) {
    double sum = 0.0;
    for (double val : x) {
        sum += val * val;
    }
    return sum;
}

// Função de teste 2: Rastrigin (mínimo global em [0,0,...,0] = 0)
double rastrigin_function(const std::vector<double>& x) {
    double A = 10.0;
    double sum = A * x.size();
    for (double val : x) {
        sum += val * val - A * std::cos(2.0 * M_PI * val);
    }
    return sum;
}

// Função de teste 3: Rosenbrock (mínimo global em [1,1,...,1] = 0)
double rosenbrock_function(const std::vector<double>& x) {
    double sum = 0.0;
    for (size_t i = 0; i < x.size() - 1; ++i) {
        double term1 = x[i+1] - x[i] * x[i];
        double term2 = x[i] - 1.0;
        sum += 100.0 * term1 * term1 + term2 * term2;
    }
    return sum;
}

void test_function(const std::string& name,
                   std::function<double(const std::vector<double>&)> func,
                   double min_bound = -5.0, double max_bound = 5.0) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "TESTANDO FUNÇÃO: " << name << std::endl;
    std::cout << std::string(50, '=') << std::endl;

    ParticleSwarmOptimization pso(30, 2, 100, func, min_bound, max_bound);
    pso.run();
    pso.print_results();
}

int main() {
    std::cout << "TESTES DO ALGORITMO PARTICLE SWARM OPTIMIZATION" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    // Teste 1: Função Esfera
    test_function("Esfera", sphere_function);

    // Teste 2: Função Rastrigin
    test_function("Rastrigin", rastrigin_function);

    // Teste 3: Função Rosenbrock
    test_function("Rosenbrock", rosenbrock_function, -2.0, 2.0);

    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "TODOS OS TESTES CONCLUÍDOS!" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    return 0;
}
