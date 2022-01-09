//
//  mathematical_model.h
//  Migration model
//
//  Created by Cibrán López Álvarez on 09/12/2021
//  Definiition of the model whose parameters we are trying to find


#ifndef mathematical_model_h
#define mathematical_model_h

#include <math.h>
#include <stdlib.h>

#define ElliotSigmoidSCALE 1000
#define TwoElliotSigmoidSCALE 2000

#define HMAX 1.0
#define HMIN 1.e-6
#define RKTOL 1.e-8


typedef struct {double phi; double beta; double lambda; double mu; double sigma; double delta;} ODE_Parameters;


double ElliotSigmoid(double x, double sigma, double delta) {
    x = sigma * (x - delta);
    return x / (ElliotSigmoidSCALE + fabs(x));
}

double Psi(double x, double mu, double sigma, double delta) {
    double ES = ElliotSigmoid(x, sigma, delta);
    sigma *= delta;
    if (x < delta) {
        x /= delta;
        ES = ES * (x + (mu * (1.0 - x) * (sigma + ElliotSigmoidSCALE)) / (sigma + TwoElliotSigmoidSCALE));
    }
    return ((1 - ES) * (sigma + TwoElliotSigmoidSCALE)) / (sigma * (1 + mu) + TwoElliotSigmoidSCALE);
}

void MigrationODE(double t, double x, double *der, void *Params){
    ODE_Parameters *par = (ODE_Parameters *) Params; // Pointer cast to save typing and thinking
    *der = par->phi * x - par->beta * x * x - par->lambda * Psi(x, par->mu, par->sigma, par->delta);
}

int Generate_EDO_Prediction(double *xt, double x0, unsigned short number_of_years, ODE_Parameters *pars) {
    register unsigned ty;
    xt[0] = x0; // Storing IC x(0)
    
    for (ty = 1; ty < number_of_years; ty++)
        xt[ty] = 0.0;
    
    double t = 0.0, err, h = 1.e-3;
    
    for (ty = 1; ty < number_of_years; ty++) {
        int status;
        while(t+h < ty) {
            status = RKF78(&t, &x0, &h, &err, HMIN, HMAX, RKTOL, pars, MigrationODE);
              if (status)
                  return status;
        } // Adaptative stepsize h. To assure stopping at t = ty
        h = ty - t;
        status = RKF78(&t, &x0, &h, &err, HMIN, HMAX, RKTOL, pars, MigrationODE);
        if (status)
            return status;
        xt[ty] = x0;
    }
    return 0;
}


#endif /* mathematical_model_h */
