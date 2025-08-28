#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <vector>
#include <functional>

class SimulatedAnnealing {
public:
    SimulatedAnnealing(std::function<double(const std::vector<double>&)> fitness_func,
                      int dimensions, double initial_temp, double final_temp,
                      int max_iterations, double cooling_rate = 0.95,
                      double min_bound = -10.0, double max_bound = 10.0);

    void run();
    std::vector<double> get_best_solution() const;
    double get_best_fitness() const;
    void print_results() const;
    void set_step_size(double step);
    void set_cooling_schedule(const std::string& schedule);

private:
    std::function<double(const std::vector<double>&)> fitness_function;
    int dimensions;
    double initial_temperature;
    double final_temperature;
    int max_iterations;
    double cooling_rate;
    double min_bound;
    double max_bound;
    double step_size;
    std::string cooling_schedule;

    std::vector<double> current_solution;
    std::vector<double> best_solution;
    double current_fitness;
    double best_fitness;
    double current_temperature;

    void initialize_solution();
    std::vector<double> generate_neighbor(const std::vector<double>& solution);
    bool accept_solution(double current_cost, double new_cost, double temperature);
    void update_temperature(int iteration);
    double calculate_temperature_linear(int iteration);
    double calculate_temperature_exponential(int iteration);
    double calculate_temperature_logarithmic(int iteration);
    void clamp_solution(std::vector<double>& solution);
};

#endif
