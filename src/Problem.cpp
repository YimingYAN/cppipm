//
//  Problem.cpp
//  cppipm
//
//  Created by Yiming Yan on 27/06/2014.
//  Copyright (c) 2014 Yiming Yan. All rights reserved.
//

#include "Problem.h"
#include <cassert>

Problem::Problem(){}

Problem::Problem(const mat &iQ, const mat &iA, const vec &ib, const vec &ic)
{
    assert(iA.n_rows > 0 && iA.n_cols > 0);
    m = iA.n_rows;
    n = iA.n_cols;
    
    Q = iQ;
    A = iA;
    b = ib;
    c = ic;
}

Problem::Problem(const mat &iA, const vec &ib, const vec &ic)
{
    assert(iA.n_rows > 0 && iA.n_cols > 0);
    
    m = iA.n_rows;
    n = iA.n_cols;
    
    A = iA;
    b = ib;
    c = ic;
    
    Q = mat(n,n, fill::zeros);
}


Problem::Problem(const Problem& otherProb)
{
    assert(otherProb.n > 0 && otherProb.m > 0);
    
    m = otherProb.m;
    n = otherProb.n;
    
    Q = otherProb.Q;
    A = otherProb.A;
    b = otherProb.b;
    c = otherProb.c;
    
}

void Problem::setOptx(const vec x)
{
    optx = x;
}

void Problem::setOpts(const vec s)
{
    optx = s;
}

void Problem::setOpty(const vec y)
{
    optx = y;
}
