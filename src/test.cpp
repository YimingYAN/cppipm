#include <iostream>
#include <armadillo>
#include <cassert>
#include <cmath>
#include "Problem.h"
#include "Parameters.h"
#include "Status.h"
#include "Iterate.h"
#include "cppipm.h"
#include "mpsReader.h"

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
    
    c(0) = 1; c(1) = 0.5;

    // Test Problems class
    Problem prob(Q,A,b,c);  //QP
    Problem lp(A,b,c);      //LP
    
    assert(Q.n_cols == prob.n && A.n_rows == prob.m);
    assert(A.n_rows == lp.m && A.n_cols == lp.n);
    
    
    // Test Parameters class
    Parameters pars;
    int verbose = 4;
    double tol = 1e-09;
    int maxIter = 10;
    
    pars.set_verbose(verbose);
    pars.set_maxIter(maxIter);
    pars.set_setTol(tol);
    
    assert((verbose == pars.verbose) && (maxIter = pars.maxIter) && (fabs(tol - pars.tol) < 1e-12 ));
    
    Parameters pars2(1,2.2,1);
    
    assert( (pars2.verbose == 1) && (pars2.maxIter == 1) &&  (fabs(pars2.tol - 2.2) < 1e-12 ) );
    
    // Test Status class
    Status stat;
    int extflg = 2;
    stat.setExitFlag(extflg);
    assert( stat.getExitFlag() == extflg);
    
    //Test Iterate class
    Iterate iter(prob);
    
    iter.initialPoint(prob);
    while (true)
    {
        iter.calResiduals(prob);
        if (iter.checkTermination(pars, stat))
        {
            break;
        }

        iter.calResiduals(prob);
        iter.solveNewton(prob);
        iter.getStepSize(pars);
        iter.updateIter();
    }
    
    // Check Status
    std::cout<<"Changed status -> "<< stat.getExitFlag()<<"\n";

    // Test the cppipm as a whole
    //cppipm qp_test = cppipm(Q, A, b, c);
    //qp_test.solve();
    
    cppipm lp_test = cppipm(A, b, c);
    lp_test.solve();
    
    
    // test read from QPS
    cout<<endl;
    cout << "test qps reader : "<<endl;
    mpsReader fileInput("/Users/yimingyan/Dropbox/Github/cppipm/examples/QAFIRO.QPS");
    
	return 0;
}
