//
//  cppipm.cpp
//  cppipm
//
//  Created by Yiming Yan on 08/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include "cppipm.h"

// Constructors
cppipm::cppipm()
{
    std:cout<<"No data input\n";
}

cppipm::cppipm(const Problem &iprob)
{
    prob = Problem(iprob);
    iter = Iterate(prob);
}

cppipm::cppipm(const Problem &iprob, const Parameters &ipars)
{
    prob = Problem(iprob);
    pars = Parameters(ipars);
    iter = Iterate(prob);
}

cppipm::cppipm(const mat &A, const vec &b, const vec &c)
{
    prob = Problem(A, b, c);
    iter = Iterate(prob);
}

cppipm::cppipm(const mat &Q, const mat &A, const vec &b, const vec &c)
{
    prob = Problem(Q, A, b, c);
    iter = Iterate(prob);
}

// Driver
void cppipm::solve()
{
    iter.initialPoint(prob);
    while (true)
    {
        iter.getResiduals(prob);
        
        if (iter.checkTermination(pars))
        {
            break;
        }
        
        iter.getResiduals(prob);
        iter.solveNewton(prob);
        iter.getStepSize(pars);
        iter.updateIter();
    }
}