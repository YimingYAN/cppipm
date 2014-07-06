#include <iostream>
#include "Problem.h"
#include "Parameters.h"

int main (int argc, char* argv[])
{
    Array2D<double> Q(2,2);
    Array2D<double> A(1,2);
    Array1D<double> b(1);
    Array1D<double> c(2);
    
    Q[0][0] = 1; Q[0][1] = 0;
    Q[1][0] = 0; Q[1][1] = 0.5;
    
    A[0][0] = 1; A[0][1] = 1;
    
    b[0] = 1;
    
    c[0] = 1; c[1] = -1;
    
    std::cout<< Q.dim1()<< "\n";
    Problem prob(Q,A,b,c);
    Problem lp(A,b,c);
    
    std::cout<< prob.m<< "\n";
    std::cout<< prob.n<< "\n";
    
    std::cout<< lp.m<< "\n";
    std::cout<< lp.n<< "\n";
    
    Parameters pars;
    
    pars.set_verbose(4);
    pars.set_maxIter(100);
    pars.set_setTol(1e-10);
    
    std::cout << "verbose = " << pars.verbose<<"; maxIter = "<<pars.maxIter<< "; tol = "<< pars.tol<<"\n";
        
    
	return 0;
}
