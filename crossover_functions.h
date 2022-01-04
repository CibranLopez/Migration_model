//
//  crossover_functions.h
//  Migration model
//
//  Created by Cibrán López Álvarez on 12/12/2021.
//


#ifndef crossover_functions_h
#define crossover_functions_h

#include "random_bits_generator.h"
#include "fitness_functions.h"


void one_point_crossover(unsigned long int p1, unsigned long int p2, unsigned long int *f1, unsigned long int *f2, int length) {
    unsigned char d = uniform() * (length - 1) + 1;
    unsigned long int mask = 0xFFFFFFFFFFFFFFFFU << d;
    
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

void crossover(population_structure population[], unsigned int pos_1, unsigned int pos_2, unsigned int i) {
    one_point_crossover(population[pos_1].x_0, population[pos_2].x_0, &population[i].x_0, &population[i+1].x_0, x_0_length);
    one_point_crossover(population[pos_1].phi, population[pos_2].phi, &population[i].phi, &population[i+1].phi, phi_length);
    one_point_crossover(population[pos_1].lambda, population[pos_2].lambda, &population[i].lambda, &population[i+1].lambda, lambda_length);
    one_point_crossover(population[pos_1].mu, population[pos_2].mu, &population[i].mu, &population[i+1].mu, mu_length);
    one_point_crossover(population[pos_1].sigma, population[pos_2].sigma, &population[i].sigma, &population[i+1].sigma, sigma_length);
    one_point_crossover(population[pos_1].delta, population[pos_2].delta, &population[i].delta, &population[i+1].delta, delta_length);
}

unsigned int get_index(void) {
    if (uniform() < crossover_probability)
        return uniform() * (1 - crossover_probability) * N_population;
    else
        return ((1 - crossover_probability) + uniform() * crossover_probability) * N_population;
}

void make_crossover(population_structure population[], unsigned int i) {
    unsigned int index_1 = get_index(), index_2 = get_index();
    crossover(population, index_1, index_2, i);
}

void mutation(unsigned long int *f, int length) {
    for (int i = 0; i < length; i++)
        if (uniform() < mutation_probability)
            *f = (*f)^(1U << i);
}

void mutate(population_structure population[], unsigned int i) {    
    mutation(&population[i].x_0, x_0_length);
    mutation(&population[i].phi, phi_length);
    mutation(&population[i].lambda, lambda_length);
    mutation(&population[i].mu, mu_length);
    mutation(&population[i].sigma, sigma_length);
    mutation(&population[i].delta, delta_length);
}


#endif /* crossover_functions_h */
