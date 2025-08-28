#ifndef PARTICLE_SWARM_OPTIMIZATION_H
#define PARTICLE_SWARM_OPTIMIZATION_H

#include <vector>
#include <functional>

struct Particle {
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> best_position;
    double fitness;
    double best_fitness;

    Particle(int dimensions);
};

class ParticleSwarmOptimization {
public:
    ParticleSwarmOptimization(int num_particles, int dimensions, int max_iterations,
                             std::function<double(const std::vector<double>&)> fitness_func,
                             double min_bound = -10.0, double max_bound = 10.0);

    void run();
    std::vector<double> get_best_solution() const;
    double get_best_fitness() const;
    void print_results() const;

private:
    int num_particles;
    int dimensions;
    int max_iterations;
    double min_bound;
    double max_bound;
    double inertia_weight;
    double cognitive_coef;
    double social_coef;

    std::vector<Particle> swarm;
    std::vector<double> global_best_position;
    double global_best_fitness;

    std::function<double(const std::vector<double>&)> fitness_function;

    void initialize_swarm();
    void evaluate_fitness();
    void update_personal_best();
    void update_global_best();
    void update_velocities();
    void update_positions();
    void clamp_positions();
};

#endif
