/* CPPIPM
 * Subclass of Algorithm
 *
 * Header for the interior point solver
 *
 * Created by Yiming Yan on 08/07/2014.
 * Copyright (c) 2014 Yiming Yan. All rights reserved.
 */

#include "Algorithm.h"
#include "Problem.h"
#include "Parameters.h"
#include "Status.h"
#include "include_libs.h"


class cppipm: public Algorithm
{
public:
    /* 
     * Overwrite the constractors
     */
    cppipm(const mat &Q, const mat &A, const vec &b, const vec &c);    // QP
    cppipm(const mat &A, const vec &b, const vec &c);                  // LP
    cppipm(const Problem &prob);
    cppipm(const Problem &prob, const Parameters &pars);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:

    Problem     prob;
    Parameters  pars;
    Status      stat;
    Eigen::BenchTimer timer;
    
    double alphax, alphas;
    double mu;
    
    double sigma;       // centring paramter
    double bc;          // regulator
    
    vec Rp, Rd;
    
    vec  x,  y,  s;
    vec dx, dy, ds;
    
    void initialize();
    void initialPoint();
    void calResidual();
    bool checkTermination();
    void calSearchDriection();
    void getStepSize();
    void updateIterates();

    void printHeader();
    void printIter();
    void printFooter();
    void startTimer();
    void endTimer();
    
    void _getDirections(vec& Rm, Factorization& factor);
    
    
};
