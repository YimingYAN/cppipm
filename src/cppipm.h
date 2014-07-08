/* CPPIPM
 * Class header for the interior point solver
 *
 *
 * Created by Yiming Yan on 08/07/2014.
 * Copyright (c) 2014 Yiming. All rights reserved.
 */
#include <iostream>
#include <armadillo>
#include <cassert>
#include <cmath>
#include "Problem.h"
#include "Parameters.h"
#include "Status.h"
#include "Iterate.h"

using namespace arma;

class cppipm
{
public:
    Problem     prob;
    Parameters  pars;
    Iterate     iter;
    Status      stat;
    
    
    // Overwrite the constractors
    cppipm();
    cppipm(const mat &Q, const mat &A, const vec &b, const vec &c);    // QP
    cppipm(const mat &A, const vec &b, const vec &c);                  // LP
    cppipm(const Problem &prob);
    cppipm(const Problem &prob, const Parameters &pars);
    
    
    // Driver function
    void solve();
    
    // Ultilities
    vec getOptx();
    vec getOpts();
    vec getOpty();
    int getIter();
    double getResiduals();
};
