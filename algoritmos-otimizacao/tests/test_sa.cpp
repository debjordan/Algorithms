#include <iostream>
#include <cmath>
#include "simulated_annealing.h"

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

// Função de teste 4: Ackley (mínimo global em [0,0,...,0] = 0)
double ackley_function(const std::vector<double>& x) {
    double a = 20.0;
    double b = 0.2;
    double c = 2.0 * M_PI;

    double sum1 = 0.0;
    double sum2 = 0.0;

    for (double val : x) {
        sum1 += val * val;
        sum2 += std::cos(c * val);
    }

    double n = static_cast<double>(x.size());
    return -a * std::exp(-b * std::sqrt(sum1 / n)) - std::exp(sum2 / n) + a + std::exp(1.0);
}

void test_function_with_schedule(const std::string& name,
                                std::function<double(const std::vector<double>&)> func,
                                const std::string& schedule,
                                double min_bound = -5.0, double max_bound = 5.0) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "TESTANDO: " << name << " | Resfriamento: " << schedule << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    SimulatedAnnealing sa(func, 2, 100.0, 0.001, 1000, 0.95, min_bound, max_bound);
    sa.set_cooling_schedule(schedule);
    sa.set_step_size(0.5);
    sa.run();
    sa.print_results();
}

void comparative_test(const std::string& name,
                     std::function<double(const std::vector<double>&)> func) {
    std::cout << "\n" << std::string(80, '#') << std::endl;
    std::cout << "TESTE COMPARATIVO: " << name << std::endl;
    std::cout << std::string(80, '#') << std::endl;

    // Teste com diferentes esquemas de resfriamento
    test_function_with_schedule(name, func, "exponential");
    test_function_with_schedule(name, func, "linear");
    test_function_with_schedule(name, func, "logarithmic");
}

int main() {
    std::cout << "TESTES DO ALGORITMO SIMULATED ANNEALING" << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    // Teste 1: Função Esfera (convexa, fácil)
    comparative_test("Esfera", sphere_function);

    // Teste 2: Função Ackley (multimodal, média dificuldade)
    comparative_test("Ackley", ackley_function);

    // Teste 3: Função Rastrigin (altamente multimodal, difícil)
    comparative_test("Rastrigin", rastrigin_function);

    // Teste específico para Rosenbrock (vale estreito)
    std::cout << "\n" << std::string(80, '#') << std::endl;
    std::cout << "TESTE ESPECÍFICO: Rosenbrock (problema do vale estreito)" << std::endl;
    std::cout << std::string(80, '#') << std::endl;

    SimulatedAnnealing sa_rosenbrock(rosenbrock_function, 2, 50.0, 0.0001, 2000, 0.98, -2.0, 2.0);
    sa_rosenbrock.set_cooling_schedule("exponential");
    sa_rosenbrock.set_step_size(0.2); // Step menor para função mais sensível
    sa_rosenbrock.run();
    sa_rosenbrock.print_results();

    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "ANÁLISE DOS RESULTADOS:" << std::endl;
    std::cout << "- Exponential: Melhor para exploração inicial" << std::endl;
    std::cout << "- Linear: Transição mais suave, bom para refinamento" << std::endl;
    std::cout << "- Logarithmic: Resfriamento mais lento, melhor para problemas complexos" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "TODOS OS TESTES CONCLUÍDOS!" << std::endl;

    return 0;
}
