//
//  cppipm.cpp
//  cppipm
//
//  Created by Yiming Yan on 08/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include "cppipm.h"
#include <iostream>
#include <iomanip>

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
    _printHeader(pars);
    
    iter.initialPoint(prob);
    
    while (true)
    {
        iter.calResiduals(prob);
        _printIter(pars, iter);
        
        if (iter.checkTermination(pars, stat))
        {
            break;
        }
        
        iter.calResiduals(prob);
        iter.solveNewton(prob);
        iter.getStepSize(pars);
        iter.updateIter();
        iter.iterIncrement();
    }
}

// Print info
void cppipm::_printHeader(const Parameters &pars)
{
    using namespace std;
    if (pars.verbose > 0)
        cout<<setw(10)<< "Iter";
        cout<<setw(10)<<"Residual";
        cout<<setw(10)<<"Mu" << "\n";
}

void cppipm::_printIter(const Parameters &pars, const Iterate &iter)
{
    using namespace std;
    
    if (pars.verbose > 0)
    {
        cout<<setw(10)<< iter.getIterNum();
        cout<<setprecision(2)<<scientific;
        cout<<setw(10)<<iter.getRes();
        cout<<setw(10)<< iter.getMu();
        cout<<"\n";
    }
}
