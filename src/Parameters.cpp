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

void Parameters::set_maxIter(int otherMaxIter)
{
    assert(otherMaxIter >= 0);
    maxIter = otherMaxIter;
}

void Parameters::set_setTol(double otehrTol)
{
    assert(otehrTol >= 0);
    tol = otehrTol;
}

void Parameters::set_verbose(int otherVerbose)
{
    assert(otherVerbose >= 0);
    verbose = otherVerbose;
}