//
//  crossover_functions.h
//  Migration model
//
//  Created by Cibrán López Álvarez on 12/12/2021
//  Definition of the funcitons for crossover and mutation


#ifndef crossover_functions_h
#define crossover_functions_h

#include "random_bits_generator.h"
#include "fitness_functions.h"


void one_point_crossover(unsigned long int p1, unsigned long int p2, unsigned long int *f1, unsigned long int *f2, int length) {
    /* Randomly selects a point for merging the chromosomes */
    unsigned char d = uniform() * (length - 1) + 1;
    unsigned long int mask = 0xFFFFFFFFFFFFFFFFUL << d;
    
    *f1 = (p1 & mask) | (p2 & ~mask);
    *f2 = (p2 & mask) | (p1 & ~mask);
}

unsigned int get_index(void) {
    /* Favoures the most fittests individuals to be selected as mates for crossover */
    if (uniform() < crossover_probability)
        return uniform() * (1 - crossover_probability) * N_population;
    else
        return ((1 - crossover_probability) + uniform() * crossover_probability) * N_population;
}

void make_crossover(population_structure population[], population_structure new_population[], unsigned int i) {
    /* Gets the indexes and applies crossover to each parameter */
    unsigned int index_1 = get_index(), index_2 = get_index();
    
    one_point_crossover(population[index_1].x_0, population[index_2].x_0, &new_population[i].x_0, &new_population[i+1].x_0, x_0_length);
    one_point_crossover(population[index_1].phi, population[index_2].phi, &new_population[i].phi, &new_population[i+1].phi, phi_length);
    one_point_crossover(population[index_1].lambda, population[index_2].lambda, &new_population[i].lambda, &new_population[i+1].lambda, lambda_length);
    one_point_crossover(population[index_1].mu, population[index_2].mu, &new_population[i].mu, &new_population[i+1].mu, mu_length);
    one_point_crossover(population[index_1].sigma, population[index_2].sigma, &new_population[i].sigma, &new_population[i+1].sigma, sigma_length);
    one_point_crossover(population[index_1].delta, population[index_2].delta, &new_population[i].delta, &new_population[i+1].delta, delta_length);
}

void one_point_mutation(unsigned long int *f, int length) {
    /* Randomly mutates a position in binary representation */
    for (int i = 0; i < length; i++)
    if (uniform() < mutation_probability)
        *f = (*f)^(1UL << (unsigned char) uniform() * length);
}

void mutate(population_structure population[], unsigned int i) {
    /* Applies mutation to each parameter */
    one_point_mutation(&population[i].x_0, x_0_length);
    one_point_mutation(&population[i].phi, phi_length);
    one_point_mutation(&population[i].lambda, lambda_length);
    one_point_mutation(&population[i].mu, mu_length);
    one_point_mutation(&population[i].sigma, sigma_length);
    one_point_mutation(&population[i].delta, delta_length);
}


#endif /* crossover_functions_h */
