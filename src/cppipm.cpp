//
//  cppipm.cpp
//  cppipm
//
//  Created by Yiming Yan on 08/07/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
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
    wall_clock timer; timer.tic();
    
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
    
    totalTime = timer.toc();
    _printFooter(pars, iter, stat);
}

// Print info
void cppipm::_printHeader(const Parameters &pars)
{
    using namespace std;
    if (pars.verbose > 0)
        cout<<setw(5)<< "Iter";
        cout<<setw(10)<<"Residual";
        cout<<setw(10)<<"Mu" << "\n";
}

void cppipm::_printIter(const Parameters &pars, const Iterate &iter)
{
    using namespace std;
    
    if (pars.verbose > 0)
    {
        cout<<setw(5)<< iter.getIterNum();
        cout<<setprecision(2)<<scientific;
        cout<<setw(10)<<iter.getRes();
        cout<<setw(10)<< iter.getMu();
        cout<<endl;
    }
}

void cppipm::_printFooter(const Parameters &pars, const Iterate &iter, const Status &stat)
{
    using namespace std;
    
    if (pars.verbose >= 0)
    {
        cout<<setfill('-')<<setw(30)<<"-"<<endl;
        cout<< "CPPIPM Terminated. ";
        cout<< "Status : "<<stat.getExitFlag() << endl;
        cout<< "[Iters: "<< iter.getIterNum()<< "] ";
        cout<<setprecision(2)<<scientific;
        cout<< "[Time: "<<totalTime << "s]";
        cout<< endl;
    }
}
