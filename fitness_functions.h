//
//  fitness_functions.h
//  Migration model
//
//  Created by Cibrán López Álvarez on 12/12/2021.
//

#ifndef fitness_functions_h
#define fitness_functions_h

#include <stdio.h>
#include <string.h>
#include "mathematical_model.h"

#define N_population 100
#define maximum_iterations 1000
#define N_years 12
#define converge_threeshold 100

#define mutation_probability 0.001
#define crossover_probability 0.7
#define crossover_proportion 0.1

#define x_0_length 21
#define phi_length 34
#define lambda_length 25
#define mu_length 25
#define sigma_length 17
#define delta_length 15

#define x_0_gen_phen 16600 / (float) (powl(2, x_0_length) - 1)
#define phi_gen_phen 100.35 / (float) (powl(2, phi_length) - 1)
#define lambda_gen_phen 3000 / (float) (powl(2, lambda_length) - 1)
#define mu_gen_phen 20 / (float) (powl(2, mu_length) - 1)
#define sigma_gen_phen 1000 / (float) (powl(2, sigma_length) - 1)
#define delta_gen_phen 25000 / (float) (powl(2, delta_length) - 1)


double x_exp[N_years] = {15329, 14177, 13031, 9762, 11271, 8688, 7571, 6983, 4778, 2067, 1586, 793};

typedef struct {unsigned long int x_0; unsigned long long int phi; unsigned long int lambda; unsigned long int mu; unsigned long int sigma; unsigned int delta; double fitness;} population_structure;


void ExitError(const char *miss, int errcode) {
    fprintf(stderr, "\nERROR: %s.\nStopping...\n\n", miss);
    exit(errcode);
}

double maximum_distance(double xt[N_years]) {
    double distance = 0, aux;
    for (int i = 0; i < N_years; i++) {
        aux = fabs(x_exp[i] - xt[i]);
        if (aux > distance)
            distance = aux;
    }
    return distance;
}

double euclidean_distance(double xt[N_years]) {
    double distance = 0;
    for (int i = 0; i < N_years; i++)
        distance += pow(x_exp[i] - xt[i], 2);
    return distance;
}

void initialize(population_structure population[], unsigned int i) {
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
    
    if (Generate_EDO_Prediction(&xt, x_0, N_years, &migration_parameters) != 0)
        printf("Unsuccessful integration\n");
    
    population[i].fitness = maximum_distance(xt);
}

void sort_by_fitness(population_structure population[]) {
    population_structure temp;
    for (unsigned int i = 0; i < N_population; i++)
        for (unsigned int j = i+1; j < N_population; j++)
            if (population[j].fitness < population[i].fitness) {
                temp = population[i];
                population[i] = population[j];
                population[j] = temp;
            }
}

int not_converged(population_structure population[], unsigned int iteration_counter) {
    if (iteration_counter >= maximum_iterations)
        return 0; // Maximum of iterations reached
    
    if (population[0].fitness <= converge_threeshold)
        return 0; // The population converged
    
    return 1; // The population did not converge
}


#endif /* fitness_functions_h */
