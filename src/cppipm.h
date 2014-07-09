/* CPPIPM
 * Header for the interior point solver
 *
 *
 * Created by Yiming Yan on 08/07/2014.
 * Copyright (c) 2014 Yiming Yan. All rights reserved.
 */
#include <armadillo>
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
    double getTime();
    
private:
    double totalTime = 0.0;
    
    //Internal functions
    void _printHeader(const Parameters &pars);
    void _printIter(const Parameters &pars, const Iterate &iter);
    void _printFooter(const Parameters &pars, const Iterate &iter, const Status &stat);
    
};
