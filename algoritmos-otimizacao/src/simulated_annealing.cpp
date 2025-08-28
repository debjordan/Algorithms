#include "simulated_annealing.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iomanip>

std::mt19937 sa_rng(static_cast<unsigned int>(std::time(nullptr)));

SimulatedAnnealing::SimulatedAnnealing(
    std::function<double(const std::vector<double>&)> fitness_func,
    int dimensions, double initial_temp, double final_temp,
    int max_iterations, double cooling_rate,
    double min_bound, double max_bound)
    : fitness_function(fitness_func), dimensions(dimensions),
      initial_temperature(initial_temp), final_temperature(final_temp),
      max_iterations(max_iterations), cooling_rate(cooling_rate),
      min_bound(min_bound), max_bound(max_bound),
      step_size(1.0), cooling_schedule("exponential"),
      current_solution(dimensions), best_solution(dimensions),
      current_fitness(std::numeric_limits<double>::max()),
      best_fitness(std::numeric_limits<double>::max()),
      current_temperature(initial_temp) {}

void SimulatedAnnealing::initialize_solution() {
    std::uniform_real_distribution<double> dist(min_bound, max_bound);

    for (int i = 0; i < dimensions; ++i) {
        current_solution[i] = dist(sa_rng);
    }

    current_fitness = fitness_function(current_solution);
    best_solution = current_solution;
    best_fitness = current_fitness;
}

std::vector<double> SimulatedAnnealing::generate_neighbor(const std::vector<double>& solution) {
    std::vector<double> neighbor = solution;
    std::uniform_real_distribution<double> dist(-step_size, step_size);
    std::uniform_int_distribution<int> dim_dist(0, dimensions - 1);

    // Modifica uma ou mais dimensões aleatoriamente
    int num_changes = std::max(1, static_cast<int>(dimensions * 0.3)); // 30% das dimensões

    for (int i = 0; i < num_changes; ++i) {
        int dim = dim_dist(sa_rng);
        neighbor[dim] += dist(sa_rng);
    }

    clamp_solution(neighbor);
    return neighbor;
}

bool SimulatedAnnealing::accept_solution(double current_cost, double new_cost, double temperature) {
    if (new_cost < current_cost) {
        return true; // Sempre aceita soluções melhores
    }

    if (temperature <= 0) {
        return false;
    }

    // Critério de aceitação de Metropolis
    double probability = std::exp(-(new_cost - current_cost) / temperature);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    return dist(sa_rng) < probability;
}

void SimulatedAnnealing::update_temperature(int iteration) {
    if (cooling_schedule == "linear") {
        current_temperature = calculate_temperature_linear(iteration);
    } else if (cooling_schedule == "exponential") {
        current_temperature = calculate_temperature_exponential(iteration);
    } else if (cooling_schedule == "logarithmic") {
        current_temperature = calculate_temperature_logarithmic(iteration);
    }

    // Garantir que a temperatura não fique menor que a final
    current_temperature = std::max(current_temperature, final_temperature);
}

double SimulatedAnnealing::calculate_temperature_linear(int iteration) {
    double alpha = static_cast<double>(iteration) / max_iterations;
    return initial_temperature * (1.0 - alpha) + final_temperature * alpha;
}

double SimulatedAnnealing::calculate_temperature_exponential(int iteration) {
    return initial_temperature * std::pow(cooling_rate, iteration);
}

double SimulatedAnnealing::calculate_temperature_logarithmic(int iteration) {
    return initial_temperature / std::log(2.0 + iteration);
}

void SimulatedAnnealing::clamp_solution(std::vector<double>& solution) {
    for (double& val : solution) {
        val = std::clamp(val, min_bound, max_bound);
    }
}

void SimulatedAnnealing::run() {
    std::cout << "Iniciando Simulated Annealing..." << std::endl;
    std::cout << "Dimensões: " << dimensions << ", Iterações: " << max_iterations << std::endl;
    std::cout << "Temperatura inicial: " << initial_temperature
              << ", Temperatura final: " << final_temperature << std::endl;
    std::cout << "Esquema de resfriamento: " << cooling_schedule << std::endl;
    std::cout << "Limites: [" << min_bound << ", " << max_bound << "]" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    initialize_solution();

    int accepted_solutions = 0;
    int total_evaluations = 1; // Já avaliamos a solução inicial

    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        // Gerar solução vizinha
        std::vector<double> neighbor = generate_neighbor(current_solution);
        double neighbor_fitness = fitness_function(neighbor);
        total_evaluations++;

        // Decidir se aceita a nova solução
        if (accept_solution(current_fitness, neighbor_fitness, current_temperature)) {
            current_solution = neighbor;
            current_fitness = neighbor_fitness;
            accepted_solutions++;

            // Atualizar melhor solução se necessário
            if (current_fitness < best_fitness) {
                best_solution = current_solution;
                best_fitness = current_fitness;
            }
        }

        // Atualizar temperatura
        update_temperature(iteration);

        // Ajustar step_size baseado na taxa de aceitação
        if (iteration % 100 == 0 && iteration > 0) {
            double acceptance_rate = static_cast<double>(accepted_solutions) / 100.0;
            if (acceptance_rate < 0.1) {
                step_size *= 0.9; // Diminuir step se aceitação muito baixa
            } else if (acceptance_rate > 0.6) {
                step_size *= 1.1; // Aumentar step se aceitação muito alta
            }
            accepted_solutions = 0; // Reset contador
        }

        // Mostrar progresso
        if (iteration % (max_iterations / 10) == 0 || iteration == max_iterations - 1) {
            std::cout << "Iteração " << std::setw(5) << iteration
                      << " | Temperatura: " << std::fixed << std::setprecision(4) << current_temperature
                      << " | Fitness atual: " << std::setprecision(6) << current_fitness
                      << " | Melhor: " << best_fitness << std::endl;
        }
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total de avaliações: " << total_evaluations << std::endl;
    std::cout << "Simulated Annealing concluído." << std::endl;
}

std::vector<double> SimulatedAnnealing::get_best_solution() const {
    return best_solution;
}

double SimulatedAnnealing::get_best_fitness() const {
    return best_fitness;
}

void SimulatedAnnealing::set_step_size(double step) {
    step_size = step;
}

void SimulatedAnnealing::set_cooling_schedule(const std::string& schedule) {
    if (schedule == "linear" || schedule == "exponential" || schedule == "logarithmic") {
        cooling_schedule = schedule;
    } else {
        std::cerr << "Esquema de resfriamento inválido. Usando 'exponential'." << std::endl;
        cooling_schedule = "exponential";
    }
}

void SimulatedAnnealing::print_results() const {
    std::cout << "\n=== RESULTADOS FINAIS ===" << std::endl;
    std::cout << "Melhor fitness encontrado: " << std::fixed << std::setprecision(8)
              << best_fitness << std::endl;
    std::cout << "Melhor solução encontrada: [";
    for (size_t i = 0; i < best_solution.size(); ++i) {
        std::cout << std::fixed << std::setprecision(4) << best_solution[i];
        if (i < best_solution.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
