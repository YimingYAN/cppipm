//
//  Iterate.h
//  cppipm
//
//  Created by Yiming Yan on 07/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#ifndef cppipm_Iterate_h
#define cppipm_Iterate_h
#include <armadillo>

#include "Problem.h"
#include "Parameters.h"
#include "Status.h"

using namespace arma;

class Iterate
{
public:
    // constructor
    Iterate(const Problem &prob, const Parameters &pars, const Status &stat);
    
    // functions
    void initialPoint(const Problem &prob);
    void getResiduals(const Problem &prob);
    bool checkTermination(const Parameters &pars);
    void solveNewton(const Problem &prob);
    void getStepSize();
    void iterIncrement();
    vec getIterx();
    vec getItery();
    vec getIters();
    
    
    
private:
    // internal properties
    int iter;
    double sigma;       // centring paramter
    double bc;          // regulator
    vec Rp;             // primal residual
    vec Rd;             // dual residual
    vec Rm;             // complementary residual
    double residual;    // residual
    double mu;          // duality gap
    
    vec    x;           // |
    vec    y;           // | iterates
    vec    s;           // |
    
    vec    dx;          // |
    vec    ds;          // | newton directions
    vec    dy;          // |
    
    double alphax;      // Step size for primal vars
    double alphas;      // Step size for dual vars
    
};



#endif
