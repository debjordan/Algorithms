#include "particle_swarm_optimization.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <limits>
#include <iomanip>

std::mt19937 pso_rng(static_cast<unsigned int>(std::time(nullptr)));

Particle::Particle(int dimensions)
    : position(dimensions), velocity(dimensions), best_position(dimensions),
      fitness(std::numeric_limits<double>::max()),
      best_fitness(std::numeric_limits<double>::max()) {}

ParticleSwarmOptimization::ParticleSwarmOptimization(
    int num_particles, int dimensions, int max_iterations,
    std::function<double(const std::vector<double>&)> fitness_func,
    double min_bound, double max_bound)
    : num_particles(num_particles), dimensions(dimensions),
      max_iterations(max_iterations), min_bound(min_bound), max_bound(max_bound),
      inertia_weight(0.9), cognitive_coef(2.0), social_coef(2.0),
      global_best_position(dimensions),
      global_best_fitness(std::numeric_limits<double>::max()),
      fitness_function(fitness_func) {

    swarm.reserve(num_particles);
    for (int i = 0; i < num_particles; ++i) {
        swarm.emplace_back(dimensions);
    }
}

void ParticleSwarmOptimization::initialize_swarm() {
    std::uniform_real_distribution<double> pos_dist(min_bound, max_bound);
    std::uniform_real_distribution<double> vel_dist(-1.0, 1.0);

    for (auto& particle : swarm) {
        for (int i = 0; i < dimensions; ++i) {
            particle.position[i] = pos_dist(pso_rng);
            particle.velocity[i] = vel_dist(pso_rng);
        }
        particle.best_position = particle.position;
    }
}

void ParticleSwarmOptimization::evaluate_fitness() {
    for (auto& particle : swarm) {
        particle.fitness = fitness_function(particle.position);
    }
}

void ParticleSwarmOptimization::update_personal_best() {
    for (auto& particle : swarm) {
        if (particle.fitness < particle.best_fitness) {
            particle.best_fitness = particle.fitness;
            particle.best_position = particle.position;
        }
    }
}

void ParticleSwarmOptimization::update_global_best() {
    for (const auto& particle : swarm) {
        if (particle.best_fitness < global_best_fitness) {
            global_best_fitness = particle.best_fitness;
            global_best_position = particle.best_position;
        }
    }
}

void ParticleSwarmOptimization::update_velocities() {
    std::uniform_real_distribution<double> rand_dist(0.0, 1.0);

    for (auto& particle : swarm) {
        for (int i = 0; i < dimensions; ++i) {
            double r1 = rand_dist(pso_rng);
            double r2 = rand_dist(pso_rng);

            double cognitive_component = cognitive_coef * r1 *
                (particle.best_position[i] - particle.position[i]);
            double social_component = social_coef * r2 *
                (global_best_position[i] - particle.position[i]);

            particle.velocity[i] = inertia_weight * particle.velocity[i] +
                cognitive_component + social_component;

            // Limitar velocidade para evitar explosão
            particle.velocity[i] = std::clamp(particle.velocity[i], -2.0, 2.0);
        }
    }
}

void ParticleSwarmOptimization::update_positions() {
    for (auto& particle : swarm) {
        for (int i = 0; i < dimensions; ++i) {
            particle.position[i] += particle.velocity[i];
        }
    }
    clamp_positions();
}

void ParticleSwarmOptimization::clamp_positions() {
    for (auto& particle : swarm) {
        for (int i = 0; i < dimensions; ++i) {
            particle.position[i] = std::clamp(particle.position[i], min_bound, max_bound);
        }
    }
}

void ParticleSwarmOptimization::run() {
    std::cout << "Iniciando Particle Swarm Optimization..." << std::endl;
    std::cout << "Partículas: " << num_particles << ", Dimensões: " << dimensions
              << ", Iterações: " << max_iterations << std::endl;
    std::cout << "Limites: [" << min_bound << ", " << max_bound << "]" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    initialize_swarm();

    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        evaluate_fitness();
        update_personal_best();
        update_global_best();

        // Mostrar progresso a cada 10 iterações
        if (iteration % 10 == 0 || iteration == max_iterations - 1) {
            std::cout << "Iteração " << std::setw(3) << iteration
                      << " | Melhor fitness: " << std::fixed << std::setprecision(6)
                      << global_best_fitness << std::endl;
        }

        update_velocities();
        update_positions();

        // Reduzir inércia ao longo do tempo
        inertia_weight = 0.9 - (0.5 * iteration / max_iterations);
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Particle Swarm Optimization concluído." << std::endl;
}

std::vector<double> ParticleSwarmOptimization::get_best_solution() const {
    return global_best_position;
}

double ParticleSwarmOptimization::get_best_fitness() const {
    return global_best_fitness;
}

void ParticleSwarmOptimization::print_results() const {
    std::cout << "\n=== RESULTADOS FINAIS ===" << std::endl;
    std::cout << "Melhor fitness encontrado: " << std::fixed << std::setprecision(8)
              << global_best_fitness << std::endl;
    std::cout << "Melhor solução encontrada: [";
    for (size_t i = 0; i < global_best_position.size(); ++i) {
        std::cout << std::fixed << std::setprecision(4) << global_best_position[i];
        if (i < global_best_position.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}
