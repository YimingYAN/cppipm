//
//  Problem.cpp
//  cppipm
//
//  Created by Yiming Yan on 27/06/2014.
//  Copyright (c) 2014 Yiming. All rights reserved.
//

#include "Problem.h"
#include <cassert>

Problem::Problem()
{
    
}

Problem::Problem(const mat &iQ, const mat &iA, const vec &ib, const vec &ic)
{
    m = iA.n_rows;
    n = iA.n_cols;
    
    assert(m>0);
    assert(n>0);
    
    Q = iQ;
    A = iA;
    b = ib;
    c = ic;
}

Problem::Problem(const mat &iA, const vec &ib, const vec &ic)
{
    m = iA.n_rows;
    n = iA.n_cols;
    
    assert(m>0);
    assert(n>0);
    
    A = iA;
    b = ib;
    c = ic;
    
    Q = mat(n,n, fill::zeros);
}


Problem::Problem(const Problem& otherProb)
{
    m = otherProb.m;
    n = otherProb.n;
    
    assert(m>0);
    assert(n>0);
    
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
