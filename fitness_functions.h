//
//  fitness_functions.h
//  Migration model
//
//  Created by Cibrán López Álvarez on 12/12/2021.
//  Definition of the fitness and other auxiliar functions, as well as all the needed parameters


#ifndef fitness_functions_h
#define fitness_functions_h

#include <stdio.h>
#include <string.h>
#include "mathematical_model.h"

#define N_population 1000
#define N_years 12

#define converge_threeshold 0.85
#define maximum_time 150

#define mutation_probability 0.001
#define crossover_probability 0.8
#define crossover_proportion 0.1

#define x_0_length 21
#define phi_length 34
#define lambda_length 25
#define mu_length 25
#define sigma_length 17
#define delta_length 15

#define beta_0 0.000024382635446

#define x_0_gen_phen 16600 / (float) (powl(2, x_0_length) - 1)
#define phi_gen_phen 100.35 / (float) (powl(2, phi_length) - 1)
#define lambda_gen_phen 3000 / (float) (powl(2, lambda_length) - 1)
#define mu_gen_phen 20 / (float) (powl(2, mu_length) - 1)
#define sigma_gen_phen 1000 / (float) (powl(2, sigma_length) - 1)
#define delta_gen_phen 25000 / (float) (powl(2, delta_length) - 1)


double x_exp[N_years] = {15329, 14177, 13031, 9762, 11271, 8688, 7571, 6983, 4778, 2067, 1586, 793};

typedef struct {unsigned long int x_0; unsigned long int phi; unsigned long int lambda; unsigned long int mu; unsigned long int sigma; unsigned long int delta; double fitness;} population_structure;


void ExitError(const char *miss, int errcode) {
    fprintf(stderr, "\nERROR: %s.\nStopping...\n\n", miss);
    exit(errcode);
}

double euclidean_distance(double xt[N_years]) {
    double distance = 0;
    for (int i = 0; i < N_years; i++)
        distance += pow(x_exp[i] - xt[i], 2);
    return distance;
}

void initialize(population_structure population[], unsigned int i) {
    /* Random numbers for the initial population */
    population[i].x_0 = N_random(x_0_length);
    population[i].phi = N_random(phi_length);
    population[i].lambda = N_random(lambda_length);
    population[i].mu = N_random(mu_length);
    population[i].sigma = N_random(sigma_length);
    population[i].delta = N_random(delta_length);
}

void compute_fitness(population_structure population[], int i) {
    /* Gets the values from the genotype and computes its integral, finally saving its fitness in the 'population' structure by using the RKF 7-8 method */
    double x_0 = population[i].x_0 * x_0_gen_phen;
    double phi = population[i].phi * phi_gen_phen - 100; // Phi has to be displaced.
    double lambda = population[i].lambda * lambda_gen_phen;
    double mu = population[i].mu * mu_gen_phen;
    double sigma = population[i].sigma * sigma_gen_phen;
    double delta = population[i].delta * delta_gen_phen;
    
    ODE_Parameters migration_parameters = {phi, beta_0, lambda, mu, sigma, delta};
    double xt[N_years];
    Generate_EDO_Prediction(&xt, x_0, N_years, &migration_parameters);
    population[i].fitness = euclidean_distance(xt);
}

void sort_by_fitness(population_structure population[]) {
    /* Interchanging individuals when a fitter one is found, and using a temporal variable */
    population_structure temp;
    for (unsigned int i = 0; i < N_population; i++)
        for (unsigned int j = i+1; j < N_population; j++)
            if (population[j].fitness < population[i].fitness) {
                temp = population[i];
                population[i] = population[j];
                population[j] = temp;
            }
}

void print_best_fitness(population_structure population[], unsigned int iteration_counter, float start) {
    /* Pinring the best result. They have to be alsready sorted */
    printf("The best solution in %u epochs and %.2f seconds is: %.2f, with parameters:\nx_0: %.2f, phi: %.2f, lambda: %.2f, mu: %.2f, sigma: %.2f, delta: %.2f\n", iteration_counter, (float) (clock() - start) / CLOCKS_PER_SEC, population[0].fitness, population[0].x_0 * x_0_gen_phen, population[0].phi * phi_gen_phen - 100, population[0].lambda * lambda_gen_phen, population[0].mu * mu_gen_phen, population[0].sigma * sigma_gen_phen, population[0].delta * delta_gen_phen);
}

int not_converged(population_structure population[], unsigned int iteration_counter, float start) {
    /* Checks if the population has converged to the best solution */
    unsigned int counter = 0;
    for (unsigned int i = 1; i < N_population; i++)
        if (population[i].fitness == population[0].fitness)
            counter++;
    
    if (counter >= converge_threeshold * N_population) {
        print_best_fitness(population, iteration_counter, start);
        return 0; // Population converged
    }
    
    if ((clock() - start) / CLOCKS_PER_SEC >= maximum_time) {
        print_best_fitness(population, iteration_counter, start);
        return 0; // Exceeded maximum execution time
    }

    return 1; // The population did not converge
}


#endif /* fitness_functions_h */
