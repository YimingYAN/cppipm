#include <iostream>
#include <cassert>
#include <cmath>
#include "Problem.h"
#include "Parameters.h"
#include <armadillo>

using namespace arma;
int main (int argc, char* argv[])
{
    mat Q(2,2);
    mat A(1,2);
    vec b(1);
    vec c(2);
    
    Q(0,0) = 1.0; Q(0,1) = 0.0;
    Q(1,0) = 0; Q(1,1) = 0.5;
    
    A(0,0) = 1; A(0,1) = 1;
    
    b(0) = 1;
    
    c(0) = 1; c(1) = -1;

    // Test Problems class
    Problem prob(Q,A,b,c);
    Problem lp(A,b,c);
    
    assert(Q.n_cols == prob.n);
    assert(A.n_rows == prob.m);
    assert(A.n_rows == lp.m);
    assert(A.n_cols == lp.n);
    
    
    // Test Parameters class
    Parameters pars;
    int verbose = 4;
    double tol = 1e-09;
    int maxIter = 10;
    
    pars.set_verbose(verbose);
    pars.set_maxIter(maxIter);
    pars.set_setTol(tol);
    
    assert(verbose == pars.verbose);
    assert(maxIter = pars.maxIter);
    assert(fabs(tol - pars.tol) < 1e-12 );
    
    Parameters pars2(1,2.2,1);
    
    assert( (pars2.verbose == 1) && (pars2.maxIter == 1) &&  (fabs(pars2.tol - 2.2) < 1e-12 ) );
    
    // Test 
    
	return 0;
}
