//
//  random_bits_generator.h
//  Migration model
//
//  Created by Cibrán López Álvarez on 09/12/2021
//  Definition of the generator of random numbers


#ifndef random_bits_generator_h
#define random_bits_generator_h

#include <time.h>

#define IA 16807
#define IM 2147483647L
#define AM (1.0/IM)
#define IQ 127773L
#define IR 2836
#define NTAB 32
#define NDIV (1 + (IM-1) / NTAB)
#define EPS 1.2e-7
#define RNMX (1.0 - EPS)


long idum;

float ran1(long *idum) {
    int j;
    long k;
    static long iy = 0, iv[NTAB];
    float temp;

    if (*idum <= 0 || !iy) {
        if (-(*idum) < 1)
            *idum=1;
        else
            *idum = -(*idum);
        
        for (j = NTAB+7; j >= 0; j--) {
            k = (*idum) / IQ;
            *idum = IA * (*idum - k * IQ) - IR * k;
            
            if (*idum < 0)
                *idum += IM;
            if (j < NTAB)
                iv[j] = *idum;
        }
        iy = iv[0];
    }
    
    k = (*idum) / IQ;
    *idum = IA * (*idum - k * IQ) - IR * k;
    
    if (*idum < 0)
        *idum += IM;
    
    j = (int) (iy / (long) NDIV);
    iy = iv[j];
    iv[j] = *idum;
    
    if ((temp = AM * iy) > RNMX)
        return RNMX;
    else
        return temp;
}


#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX


/* Between 0.0 and 1.0 */
float uniform(void) {
    return ran1(&idum);
}

/* Based on von-neuman observation; rather inefficient */
unsigned char random_bit(void) {
    unsigned char f, s;
    do {
        f = 2 * ran1(&idum);
        s = 2 * ran1(&idum);
    } while (f == s);
    return f;
}

unsigned long int N_random(int N){
    /* Getting random numbers of order N */
    register unsigned char i;
    unsigned long int oneUL = 1UL, base = 0UL;

    for (i = 0; i < N; i++)
        if (random_bit()) {
            base = oneUL;
            break;
        }
    for (i++; i < N; i++) {
        base = base << 1;
        if (random_bit())
            base = base | oneUL;
    }
    return base;
}


#endif /* random_bits_generator_h */
