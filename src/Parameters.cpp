//
//  Parameters.cpp
//  cppipm
//
//  Created by Yiming Yan on 06/07/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//
#include <cassert>
#include "Parameters.h"


Parameters::Parameters()
{
    maxIter = 50;
    tol = 1e-07;
    verbose = 1;
}

Parameters::Parameters(int otherMaxIter, double otherTol, int otherVerbose)
{
    assert(otherMaxIter >= 0);
    maxIter = otherMaxIter;
    
    assert(otherTol >= 0);
    tol = otherTol;
    
    assert(otherVerbose >= 0);
    verbose = otherVerbose;
}

void Parameters::set_maxIter(int otherMaxIter)
{
    assert(otherMaxIter >= 0);
    maxIter = otherMaxIter;
}

void Parameters::set_setTol(double otherTol)
{
    assert(otherTol >= 0);
    tol = otherTol;
}

void Parameters::set_verbose(int otherVerbose)
{
    assert(otherVerbose >= 0);
    verbose = otherVerbose;
}