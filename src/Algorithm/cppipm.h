/* CPPIPM
 * Subclass of Algorithm
 *
 * Header for the interior point solver
 *
 * Created by Yiming Yan on 08/07/2014.
 * Copyright (c) 2014 Yiming Yan. All rights reserved.
 */

#include <Core/Algorithm.h>
#include <Core/Problem.h>
#include <Core/Parameters.h>
#include <Core/Status.h>
#include <Utilities/mpsReader.h>
#include "global.h"

class Problem;
class Parameters;
class Status;

class DLL_EXPORT cppipm: public Algorithm
{
public:
    /* 
     * Overwrite the constractors
     */
    cppipm(const mat &Q, const mat &A, const vec &b, const vec &c);    // QP
    cppipm(const mat &A, const vec &b, const vec &c);                  // LP
    cppipm(const Problem &m_prob);
    cppipm(const Problem &m_prob, const Parameters &m_params);
    cppipm(const string fileName);                                     // mps file

private:
    Problem     m_prob;
    Parameters  m_params;
    Status      m_stat;
    Eigen::BenchTimer m_timer;
    
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
    
    void getDirections(vec& Rm, Factorization& factor);
};
