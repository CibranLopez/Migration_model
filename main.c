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


/* Here the main algorithm beggins: it only needs the definition of the search range for each variable, as well as the number of individuals in the population */
int main() {
    unsigned int iteration_counter = 0;
    
    /* Initializing the population structure */
    population_structure *population;
    if ((population = (population_structure *) malloc(N_population * sizeof(population_structure))) == NULL)
        ExitError("When allocating memory for the population vector", 1);
    
    for (unsigned int i = 0; i < N_population; i++) {
        /* Generating initial population and saving it to a string with always the same length */
        initialize(population, i);
        
        /* Computing the initial fitness */
        compute_fitness(population, i);
    }
    
    /* Getting the order by fitness */
    sort_by_fitness(population);
    
    /* Starting the main loop */
    while (not_converged(population, iteration_counter)) {
        printf("\n----------------\n\nNew epoch: %u\n\n", iteration_counter++);
        
        /* Applying crossover */
        for (unsigned int i = N_population * crossover_proportion; i < N_population - 1; i += 2)
            make_crossover(population, i);
        
        for (unsigned int i = 0; i < N_population; i++) {
            /* Applying mutation */
            mutate(population, i);
            
            /* Computing the new fitness */
            compute_fitness(population, i);
        }
        
        /* Getting the order by fitness */
        sort_by_fitness(population);
        
        printf("The best solution is: %f with parameters:\n", population[0].fitness);
        printf("x_0: %f, phi: %f, lambda: %f, mu: %f, sigma: %f, delta: %f\n", population[0].x_0*x_0_gen_phen, population[0].phi*phi_gen_phen-100, population[0].lambda*lambda_gen_phen, population[0].mu*mu_gen_phen, population[0].sigma*sigma_gen_phen, population[0].delta*delta_gen_phen);
        
    }
}
