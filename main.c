//
//  main.c
//  Migration model
//
//  Created by Cibrán López Álvarez on 09/12/2021.
//


#include "RKF78.h"
#include "crossover_functions.h"
#include "mathematical_model.h"
#include "random_bits_generator.h"
#include "fitness_functions.h"


/* Here the main algorithm beggins: it only needs the definition of the search range for each variable, as well as the number of individuals in the population and the minimum and maximum number of steps */
int main() {
    unsigned int iteration_counter = 1;
    clock_t start;
    start = clock();
    
    /* Initializing the population structure */
    population_structure *population;
    
    if ((population = (population_structure *) malloc(N_population * sizeof(population_structure))) == NULL)
        ExitError("When allocating memory for the population vector", 1);
    
    for (unsigned int i = 0; i < N_population; i++) {
        initialize(population, i); // Generating initial population
        compute_fitness(population, i); // Computing the initial fitness
    }
    
    sort_by_fitness(population); // Getting the order by fitness
    
    /* Starting the main loop */
    while (not_converged(population, iteration_counter, start)) {
        iteration_counter++;
        
        for (unsigned int i = N_population * crossover_proportion; i < N_population - 1; i += 2)
            make_crossover(population, population, i); // Applying crossover
        
        for (unsigned int i = 0; i < N_population; i++) {
            mutate(population, i); // Applying mutation
            compute_fitness(population, i); // Computing the new fitness
        }
        
        sort_by_fitness(population); // Getting the order by fitness
    }
}
